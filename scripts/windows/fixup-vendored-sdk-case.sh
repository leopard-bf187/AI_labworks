#!/usr/bin/env bash
# Normalizes filename casing inside a vendored MSVC/WinSDK tree so it can be
# cross-compiled against from a case-sensitive filesystem (Linux CI).
#
# The __LOCAL_DEVENV__ archive (MSVC 14.29.30133 + Windows SDK 10.0.17763.0)
# was packaged for Windows, where the filesystem is case-insensitive. Some
# files inside it were re-cased inconsistently during export (e.g. actual
# file "kernel32.Lib" but every #include/link reference expects
# "kernel32.lib"; actual "driverspecs.h" but referenced as "DriverSpecs.h").
# On a case-sensitive filesystem this breaks both the linker (missing .lib)
# and the compiler (missing header), even though every byte of content is
# present under a different name.
#
# This script does NOT rename or modify any original file — it only adds
# symlinks for the casings that are actually referenced, so the vendored
# copy stays diffable against the original archive.
#
# Usage: fixup-vendored-sdk-case.sh <path-to-msvc16_winsdk17763-root>
# (a directory containing msvc/ and winsdk/ subdirectories)
#
# Re-run this after any re-sync of the vendored SDK from a fresh archive
# export.

set -euo pipefail

ROOT="${1:?Usage: $0 <path-to-msvc16_winsdk17763-root>}"
cd "$ROOT"

if [[ ! -d msvc || ! -d winsdk ]]; then
    echo "error: $ROOT does not contain msvc/ and winsdk/ directories" >&2
    exit 1
fi

echo "== Pass 1: lowercase alias for every file (fixes case-only mismatches like kernel32.Lib -> kernel32.lib) =="
lower_count=0
while IFS= read -r -d '' f; do
    dir=$(dirname "$f")
    base=$(basename "$f")
    lower=$(printf '%s' "$base" | tr '[:upper:]' '[:lower:]')
    if [[ "$base" != "$lower" && ! -e "$dir/$lower" ]]; then
        ln -s "$base" "$dir/$lower"
        lower_count=$((lower_count + 1))
    fi
done < <(find winsdk msvc -type f -print0)
echo "created $lower_count lowercase symlinks"

echo "== Pass 2: exact-casing alias for every #include actually referenced inside the SDK headers =="
tmp_requested=$(mktemp)
trap 'rm -f "$tmp_requested"' EXIT
grep -rhoE '#[[:space:]]*include[[:space:]]*["<][^">]+[">]' \
    winsdk/Include "msvc/Tools/"*/include 2>/dev/null \
    | sed -E 's/.*["<]([^">]+)[">].*/\1/' \
    | sed -E 's#.*/##' \
    | sort -u >"$tmp_requested"

alias_count=0
while IFS= read -r reqname; do
    [[ -z "$reqname" ]] && continue
    while IFS= read -r -d '' match; do
        dir=$(dirname "$match")
        if [[ ! -e "$dir/$reqname" ]]; then
            ln -s "$(basename "$match")" "$dir/$reqname"
            alias_count=$((alias_count + 1))
        fi
    done < <(find winsdk msvc -iname "$reqname" -print0 2>/dev/null)
done <"$tmp_requested"
echo "created $alias_count exact-casing alias symlinks"
