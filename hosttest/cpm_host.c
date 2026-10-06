/*
 * Runs a CP/M .COM CPU test (8080PRE, 8080EXM, TST8080, CPUTEST) on the
 * i8085 core, to check the port before anything is built on top of it.
 *
 *   ./hosttest/cpm_host [--8085] <file.com>...
 *
 * The 8080 exercisers check 8080 flag behaviour, so the default is 8080 mode;
 * --8085 runs the same binaries with the 8085 differences switched on.
 *
 * BDOS is emulated the way ares' test does it: 0x0000 holds OUT 0 (exit) and
 * 0x0005 holds OUT 1 / RET (print). Functions 2 (char) and 9 (string) only.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "i8085.h"

static uint8_t ram[0x10000];
static int done;

static uint8_t mem_read(i8085_t *c, uint16_t a)
{
    (void)c;
    return ram[a];
}

static void mem_write(i8085_t *c, uint16_t a, uint8_t v)
{
    (void)c;
    ram[a] = v;
}

static uint8_t io_in(i8085_t *c, uint8_t port)
{
    (void)c;
    (void)port;
    return 0xff;
}

static void io_out(i8085_t *c, uint8_t port, uint8_t v)
{
    (void)v;
    if (port == 0)
    {
        done = 1;
        return;
    }
    if (port == 1)
    {
        if (c->bc.b.l == 2)
            putchar(c->de.b.l);
        else if (c->bc.b.l == 9)
            for (uint16_t a = c->de.w; ram[a] != '$'; a++)
                putchar(ram[a]);
        fflush(stdout);
    }
}

static int run_test(const char *path, int is_8085)
{
    FILE *f = fopen(path, "rb");
    if (!f)
    {
        fprintf(stderr, "cannot open %s\n", path);
        return 1;
    }
    memset(ram, 0, sizeof(ram));
    size_t n = fread(ram + 0x100, 1, sizeof(ram) - 0x100, f);
    fclose(f);

    ram[0x0000] = 0xd3; /* OUT 0 */
    ram[0x0001] = 0x00;
    ram[0x0005] = 0xd3; /* OUT 1 */
    ram[0x0006] = 0x01;
    ram[0x0007] = 0xc9; /* RET */

    i8085_t cpu;
    i8085_init(&cpu, is_8085);
    for (int p = 0; p < 256; p++)
    {
        cpu.rd_page[p] = &ram[p << 8];
        cpu.wr_page[p] = &ram[p << 8];
    }
    cpu.read = mem_read;
    cpu.write = mem_write;
    cpu.in = io_in;
    cpu.out = io_out;
    i8085_reset(&cpu);
    cpu.pc.w = 0x100;

    printf("---- %s (%zu bytes, %s mode) ----\n", path, n, is_8085 ? "8085" : "8080");
    done = 0;
    unsigned long long cycles = 0;
    while (!done)
    {
        cpu.icount = 0;
        i8085_run(&cpu, 1);
        cycles += 1 - cpu.icount;
    }
    printf("\n---- done, %llu cycles ----\n\n", cycles);
    return 0;
}

int main(int argc, char **argv)
{
    int is_8085 = 0;
    int rc = 0;
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "--8085") == 0)
            is_8085 = 1;
        else
            rc |= run_test(argv[i], is_8085);
    }
    return rc;
}
