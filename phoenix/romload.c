/*
 * Phoenix ROM set loader. See romload.h.
 */
#include "romload.h"

#include <stdlib.h>
#include <string.h>

#include "miniz.h"

/* MAME ROM_START(phoenix) */
const phx_romfile_t phx_romfiles[PHX_ROMFILE_COUNT] = {
    {"ic45", 0x9f68086b, 0x0800, 0, 0x0000},
    {"ic46", 0x273a4a82, 0x0800, 0, 0x0800},
    {"ic47", 0x3d4284b9, 0x0800, 0, 0x1000},
    {"ic48", 0xcb5d9915, 0x0800, 0, 0x1800},
    {"h5-ic49.5a", 0xa105e4e7, 0x0800, 0, 0x2000},
    {"h6-ic50.6a", 0xac5e9ec1, 0x0800, 0, 0x2800},
    {"h7-ic51.7a", 0x2eab35b4, 0x0800, 0, 0x3000},
    {"h8-ic52.8a", 0xaff8e9c5, 0x0800, 0, 0x3800},
    {"ic23.3d", 0x3c7e623f, 0x0800, 1, 0x0000},
    {"ic24.4d", 0x59916d3b, 0x0800, 1, 0x0800},
    {"b1-ic39.3b", 0x53413e8f, 0x0800, 2, 0x0000},
    {"b2-ic40.4b", 0x0be2ba91, 0x0800, 2, 0x0800},
    {"mmi6301.ic40", 0x79350b25, 0x0100, 3, 0x0000},
    {"mmi6301.ic41", 0xe176b768, 0x0100, 3, 0x0100},
};

static uint8_t *dest_of(phx_roms_t *roms, const phx_romfile_t *f)
{
    switch (f->region)
    {
    case 0:
        return roms->maincpu + f->offset;
    case 1:
        return roms->bgtiles + f->offset;
    case 2:
        return roms->fgtiles + f->offset;
    default:
        return roms->proms + f->offset;
    }
}

/* Index of the still-missing set file with this size and CRC, or -1. */
static int wanted(uint32_t found, uint32_t size, uint32_t crc)
{
    for (int i = 0; i < PHX_ROMFILE_COUNT; i++)
        if (!(found & (1u << i)) && phx_romfiles[i].size == size && phx_romfiles[i].crc == crc)
            return i;
    return -1;
}

static uint32_t crc32_of(const uint8_t *p, size_t n)
{
    return (uint32_t)mz_crc32(MZ_CRC32_INIT, p, n);
}

static uint16_t le16(const uint8_t *p)
{
    return (uint16_t)(p[0] | (p[1] << 8));
}

static uint32_t le32(const uint8_t *p)
{
    return (uint32_t)p[0] | ((uint32_t)p[1] << 8) | ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
}

static int ends_with_ci(const char *s, const char *suffix)
{
    size_t n = strlen(s), m = strlen(suffix);
    if (m > n)
        return 0;
    s += n - m;
    for (size_t i = 0; i < m; i++)
    {
        char a = s[i], b = suffix[i];
        if (a >= 'A' && a <= 'Z')
            a += 'a' - 'A';
        if (b >= 'A' && b <= 'Z')
            b += 'a' - 'A';
        if (a != b)
            return 0;
    }
    return 1;
}

/* ---------------------------------------------------------------------------
 * Zip
 * ------------------------------------------------------------------------ */

#define ZIP_EOCD_SIG 0x06054b50u
#define ZIP_CDH_SIG 0x02014b50u
#define ZIP_LFH_SIG 0x04034b50u
#define ZIP_EOCD_SEARCH 1024 /* covers a TorrentZip comment and then some */

/* Extracts one entry to dst (exactly `size` bytes), verifying the CRC. */
static int zip_extract(const phx_io_t *io, void *fh, uint32_t lfh_ofs, int method, uint32_t comp_size,
                       uint8_t *dst, uint32_t size, uint32_t crc)
{
    uint8_t lfh[30];
    if (io->read_at(io->ctx, fh, lfh_ofs, lfh, sizeof(lfh)) != (int)sizeof(lfh) || le32(lfh) != ZIP_LFH_SIG)
        return 0;
    uint32_t data_ofs = lfh_ofs + 30 + le16(lfh + 26) + le16(lfh + 28);

    uint8_t *comp = (uint8_t *)malloc(comp_size ? comp_size : 1);
    if (!comp)
        return 0;
    int ok = 0;
    if (io->read_at(io->ctx, fh, data_ofs, comp, comp_size) == (int)comp_size)
    {
        if (method == 0 && comp_size == size)
        {
            memcpy(dst, comp, size);
            ok = 1;
        }
        else if (method == 8)
        {
            tinfl_decompressor *d = tinfl_decompressor_alloc();
            if (d)
            {
                size_t in_sz = comp_size, out_sz = size;
                tinfl_status st = tinfl_decompress(d, comp, &in_sz, dst, dst, &out_sz,
                                                   TINFL_FLAG_USING_NON_WRAPPING_OUTPUT_BUF);
                ok = (st == TINFL_STATUS_DONE && out_sz == size);
                tinfl_decompressor_free(d);
            }
        }
    }
    free(comp);
    return ok && crc32_of(dst, size) == crc;
}

static uint32_t load_zip(phx_roms_t *roms, const phx_io_t *io, const char *path, uint32_t found)
{
    void *fh = io->open(io->ctx, path);
    if (!fh)
        return found;

    uint32_t fsize = io->size(io->ctx, fh);
    uint32_t tail = fsize < ZIP_EOCD_SEARCH ? fsize : ZIP_EOCD_SEARCH;
    uint8_t *buf = (uint8_t *)malloc(tail);
    if (!buf || tail < 22 || io->read_at(io->ctx, fh, fsize - tail, buf, tail) != (int)tail)
    {
        free(buf);
        io->close(io->ctx, fh);
        return found;
    }

    /* find the end of central directory record, last one wins */
    int eocd = -1;
    for (int i = (int)tail - 22; i >= 0; i--)
    {
        if (le32(buf + i) == ZIP_EOCD_SIG)
        {
            eocd = i;
            break;
        }
    }
    uint32_t entries = 0, cd_ofs = 0;
    if (eocd >= 0)
    {
        entries = le16(buf + eocd + 10);
        cd_ofs = le32(buf + eocd + 16);
    }
    free(buf);

    uint32_t ofs = cd_ofs;
    for (uint32_t e = 0; e < entries && found != PHX_ROMS_ALL; e++)
    {
        uint8_t cdh[46];
        if (io->read_at(io->ctx, fh, ofs, cdh, sizeof(cdh)) != (int)sizeof(cdh) || le32(cdh) != ZIP_CDH_SIG)
            break;
        int method = le16(cdh + 10);
        uint32_t crc = le32(cdh + 16);
        uint32_t comp_size = le32(cdh + 20);
        uint32_t size = le32(cdh + 24);
        uint32_t lfh_ofs = le32(cdh + 42);
        ofs += 46 + le16(cdh + 28) + le16(cdh + 30) + le16(cdh + 32);

        int idx = wanted(found, size, crc);
        if (idx < 0)
            continue;
        if (zip_extract(io, fh, lfh_ofs, method, comp_size, dest_of(roms, &phx_romfiles[idx]), size, crc))
            found |= 1u << idx;
    }

    io->close(io->ctx, fh);
    return found;
}

/* ---------------------------------------------------------------------------
 * Loose files
 * ------------------------------------------------------------------------ */

static uint32_t load_loose(phx_roms_t *roms, const phx_io_t *io, const char *path, uint32_t size, uint32_t found)
{
    void *fh = io->open(io->ctx, path);
    if (!fh)
        return found;
    uint8_t *buf = (uint8_t *)malloc(size);
    if (buf && io->read_at(io->ctx, fh, 0, buf, size) == (int)size)
    {
        int idx = wanted(found, size, crc32_of(buf, size));
        if (idx >= 0)
        {
            memcpy(dest_of(roms, &phx_romfiles[idx]), buf, size);
            found |= 1u << idx;
        }
    }
    free(buf);
    io->close(io->ctx, fh);
    return found;
}

/* ---------------------------------------------------------------------------
 * Directory scan
 * ------------------------------------------------------------------------ */

#define MAX_CANDIDATES 48
#define MAX_NAME 96

typedef struct
{
    char name[MAX_NAME];
    uint32_t size;
    uint8_t is_zip;
} candidate_t;

typedef struct
{
    candidate_t *list;
    int count;
} scan_t;

static void scan_cb(void *arg, const char *name, uint32_t size, int is_dir)
{
    scan_t *s = (scan_t *)arg;
    if (is_dir || s->count >= MAX_CANDIDATES || strlen(name) >= MAX_NAME)
        return;
    int is_zip = ends_with_ci(name, ".zip");
    if (!is_zip && size != 0x800 && size != 0x100)
        return; /* not a Phoenix ROM by size */
    candidate_t *c = &s->list[s->count++];
    strcpy(c->name, name);
    c->size = size;
    c->is_zip = (uint8_t)is_zip;
}

static void join(char *out, size_t n, const char *dir, const char *name)
{
    size_t dl = strlen(dir);
    if (dl && dir[dl - 1] == '/')
        dl--;
    if (dl + 1 + strlen(name) + 1 > n)
    {
        out[0] = 0;
        return;
    }
    memcpy(out, dir, dl);
    out[dl] = '/';
    strcpy(out + dl + 1, name);
}

uint32_t phx_romload(phx_roms_t *roms, const phx_io_t *io, const char *dir, uint32_t found)
{
    scan_t s;
    s.count = 0;
    s.list = (candidate_t *)malloc(sizeof(candidate_t) * MAX_CANDIDATES);
    if (!s.list)
        return found;
    if (!io->list_dir(io->ctx, dir, scan_cb, &s))
    {
        free(s.list);
        return found;
    }

    char path[MAX_NAME * 2 + 64];

    /* pass 0: phoenix.zip, pass 1: other zips, pass 2: loose files */
    for (int pass = 0; pass < 3 && found != PHX_ROMS_ALL; pass++)
    {
        for (int i = 0; i < s.count && found != PHX_ROMS_ALL; i++)
        {
            candidate_t *c = &s.list[i];
            int is_phoenix_zip = c->is_zip && ends_with_ci(c->name, "phoenix.zip") && strlen(c->name) == 11;
            if ((pass == 0 && !is_phoenix_zip) || (pass == 1 && (!c->is_zip || is_phoenix_zip)) ||
                (pass == 2 && c->is_zip))
                continue;
            join(path, sizeof(path), dir, c->name);
            if (!path[0])
                continue;
            found = c->is_zip ? load_zip(roms, io, path, found) : load_loose(roms, io, path, c->size, found);
        }
    }

    free(s.list);
    return found;
}
