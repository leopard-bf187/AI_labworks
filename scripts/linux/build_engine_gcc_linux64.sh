#!/usr/bin/env bash
set -e
cmake --preset gcc_linux64
cmake --build --preset build_gcc_linux64
cmake --install projects/gcc_linux64
