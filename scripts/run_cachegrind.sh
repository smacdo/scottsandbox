#!/bin/bash
currtime=`date +"%Y_%m_%d-%H_%M_%S"`
filepath="logs/cachegrind/$currtime.txt"
branchsim="yes"

echo "Running cache/callgrind, log is located: $filepath"

mkdir -p logs/cachegrind
valgrind --log-file=$filepath --tool=callgrind --branch-sim=$branchsim $1
