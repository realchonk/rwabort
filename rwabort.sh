#!/bin/sh

lib='@prefix@/lib/librwabort.so'

exec env LD_PRELOAD="$lib" "$@"
