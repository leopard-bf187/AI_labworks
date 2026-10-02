#!/usr/bin/env bash
set -e
cmake --preset gcc_linux32d
cmake --build --preset build_gcc_linux32d
cmake --install projects/gcc_linux32d
