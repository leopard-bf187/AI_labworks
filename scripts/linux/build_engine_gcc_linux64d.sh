#!/usr/bin/env bash
set -e
cmake --preset gcc_linux64d
cmake --build --preset build_gcc_linux64d
cmake --install projects/gcc_linux64d
