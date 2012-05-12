#!/bin/bash
currtime=`date +"%Y_%m_%d-%H_%M_%S"`
filepath="logs/memgrind/$currtime.txt"
branchsim="yes"

echo "Running memgrind, log is located: $filepath"

mkdir -p logs/memgrind
valgrind --log-file=$filepath --tool=memcheck $1
