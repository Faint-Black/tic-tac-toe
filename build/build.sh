#!/bin/bash

cmake ..
cmake --build . --target=clang-format
make -j
