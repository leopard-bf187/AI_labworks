#!/usr/bin/env bash
set -e
cmake --preset gcc_linux32
cmake --build --preset build_gcc_linux32
cmake --install projects/gcc_linux32
