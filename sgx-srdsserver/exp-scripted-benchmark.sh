#!/bin/bash

set -x

for i in {1..100}
do
   ./exp-benchmark-noSGX.sh 4 4 30 "$i"
   ./exp-benchmark-SGX.sh 4 4 30 "$i"
done

source exp-stop.sh

