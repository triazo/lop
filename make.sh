#!/bin/bash

clang -S -emit-llvm lib/binopts.cpp -o binops.ll -march=none

# ./opt -S -strip binops.ll -o binops.ll
# ./opt -S -strip $1 -o s1.ll

llvm-link binops.ll $1 -o=s2.ll
#cat s1.ll | ./opt -S -load ./irep.so -ireplace >out.ll
