#!/bin/sh
# This script is used to build the project

set -xe

gcc -g *.c -o repl
./repl
