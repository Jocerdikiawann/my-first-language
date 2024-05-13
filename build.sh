#!/bin/sh
# This script is used to build the project

set -xe

gcc -g -O3 *.c -o repl
./repl
