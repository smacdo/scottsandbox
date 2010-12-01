#!/bin/bash
currtime=`date +"%Y_%m_%d-%H_%M_%S"`
filepath="logs/heapgrind/$currtime.txt"
branchsim="yes"

echo "Running headgrind (massif), log is located: $filepath"

mkdir -p logs/cachegrind
valgrind --log-file=$filepath --tool=massif $1
