#!/usr/bin/env bash
# Build the host harnesses:
#   hosttest/phx_host  - the Phoenix core (machine, video, sound, ROM loader)
#   hosttest/cpm_host  - the i8085 core on CP/M CPU tests
#
# The core in phoenix/ has no Pico dependencies; this compiles it for Linux
# with ASan. Drop -fsanitize=address (and use -O2) for timing measurements:
#   OPT="-O2" hosttest/build.sh
set -euo pipefail
cd "$(dirname "$0")/.."

OPT=${OPT:--O1 -g -fsanitize=address -fno-omit-frame-pointer}
CFLAGS=(
  $OPT -std=c11 -Wall -Wextra -Wno-unused-parameter
  -D_DEFAULT_SOURCE
  -I phoenix -I third_party/miniz
  -DMINIZ_NO_STDIO -DMINIZ_NO_TIME -DMINIZ_NO_ARCHIVE_APIS -DMINIZ_NO_DEFLATE_APIS
  -DMINIZ_NO_ZLIB_APIS -DMINIZ_NO_ZLIB_COMPATIBLE_NAMES
)

CORE=(
  phoenix/i8085.c
  phoenix/phoenix.c
  phoenix/phoenix_sound.c
  phoenix/tms36xx.c
  phoenix/romload.c
  third_party/miniz/miniz.c
)

gcc "${CFLAGS[@]}" hosttest/phx_host.c "${CORE[@]}" -lm -o hosttest/phx_host
gcc "${CFLAGS[@]}" hosttest/cpm_host.c phoenix/i8085.c -o hosttest/cpm_host
echo "built hosttest/phx_host and hosttest/cpm_host"
