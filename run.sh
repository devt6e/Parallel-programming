#!/bin/bash

prog=hw02
src=hw01_2.c

gcc -O2 $src -o $prog

dims=(10000 100000)
ITER=10000
strides=$(seq 1 16)

for d in "${dims[@]}"; do
  for s in $strides; do
    echo "Running with DIM=$d, ITER=$ITER, stride=$s"
    # ./$prog $d $ITER $s
    # ./$prog $d $ITER $s > result_${d}_${ITER}_${s}.txt
    ./$prog $d $ITER $s | tee -a all_results_2_O0.txt
  done
  echo
done
