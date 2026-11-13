#!/usr/bin/env bash
#
# Builds jemalloc for linking into the shared library of Konclude (KoncludeLIB.pro with
# CONFIG+=jemalloc), and copies its position independent archive to the given directory.
#
# Usage: build-jemalloc-library.sh <work directory> <output directory> [debug]
#
# The library is loaded into a program that is already running, the Java virtual machine, so
# jemalloc's initial-exec thread local storage is switched off: a library loaded with dlopen
# gets no initial-exec TLS block of that size ("cannot allocate memory in static TLS block").
# Its C++ operators are left out, the C++ runtime linked into the library brings its own, which
# call malloc and so end up in jemalloc as well. With 'debug', jemalloc checks its arguments and
# aborts on a pointer it did not allocate, which is how a free of memory that the C library
# allocated shows up in the tests. See 'THE MEMORY ALLOCATOR ON LINUX' in Java/Readme.md.
set -euo pipefail

WORK=$1
OUT=$2
VARIANT=${3:-release}
JEMALLOC_VERSION=5.2.1
JOBS=$(nproc 2>/dev/null || sysctl -n hw.ncpu)

mkdir -p "$WORK" "$OUT"
WORK=$(cd "$WORK" && pwd)
OUT=$(cd "$OUT" && pwd)
cd "$WORK"
curl -fsSL --retry 5 -o jemalloc.tar.bz2 \
	"https://github.com/jemalloc/jemalloc/releases/download/$JEMALLOC_VERSION/jemalloc-$JEMALLOC_VERSION.tar.bz2"
tar -xjf jemalloc.tar.bz2
cd "jemalloc-$JEMALLOC_VERSION"

OPTIONS=(--disable-initial-exec-tls --disable-cxx)
# the same page size as the build for the command line, see build-static-dependencies.sh
if [ "$(uname -s)-$(uname -m)" = "Linux-aarch64" ]; then
	OPTIONS+=(--with-lg-page=16)
fi
if [ "$VARIANT" = debug ]; then
	OPTIONS+=(--enable-debug)
fi
./configure --prefix="$WORK/jemalloc-library" "${OPTIONS[@]}"
make -j"$JOBS" build_lib_static
cp lib/libjemalloc_pic.a "$OUT/"
ls -l "$OUT/libjemalloc_pic.a"
