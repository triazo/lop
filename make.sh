#!/bin/bash

clang -S -emit-llvm lib/binopts.cpp -o binops.ll

# ./opt -S -strip binops.ll -o binops.ll
# ./opt -S -strip $1 -o s1.ll
sed -i '/datalayout/d' binops.ll
sed -i '/triple/d' binops.ll
sed -i '/attributes/d' binops.ll

llvm-link -S binops.ll $1 -o=s2.ll
./opt -S -load ./irep.so -ireplace  <s2.ll >out.ll
