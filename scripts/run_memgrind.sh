#!/bin/bash
path=$1
app=$2
out=$3
branchsim="yes"

echo "RUNNING VALGRIND WITH $path/$app"
echo "LOG: $out"

mkdir -p $path
mkdir -p $path/valgrind

valgrind \
    --tool=memcheck                         \
    --trace-children=yes                    \
    --child-silent-after-fork=yes           \
    --log-file=$path/valgrind/$out_%p.txt   \
    --xml=yes                               \
    --xml-file=$path/valgrind/$out_%p.xml   \
    $path/$app
