#!/bin/bash

set -x

for i in {1..20}
do
   ./exp-benchmark.sh 1 1 60 "$i"
done

#for i in {1..20}
#do
#   ./exp-benchmark.sh 2 2 60 "$i"
#done

#for i in {1..20}
#do
#   ./exp-benchmark.sh 2 3 60 "$i"
#done

#for i in {1..20}
#do
#   ./exp-benchmark.sh 2 4 60 "$i"
#done

#for i in {1..20}
#do
#   ./exp-benchmark.sh 3 3 60 "$i"
#done

#for i in {1..20}
#do
#   ./exp-benchmark.sh 3 4 60 "$i"
#done

#for i in {1..20}
#do
#   ./exp-benchmark.sh 4 4 60 "$i"
#done

source exp-stop.sh

