#!/bin/sh
make -C .. clean
make -C .. test
export BLOCK_SIZE=64
../test_unitaire 1 10E-8 100
gnuplot plot.gpl
