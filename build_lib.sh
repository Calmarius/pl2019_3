#!/bin/bash

g++ -c -g -O0 -Wall -Wextra -fmax-errors=1 _.cpp -o _.o
ar rcs libpfx.a _.o

