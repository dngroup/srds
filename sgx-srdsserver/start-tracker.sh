#!/bin/bash

set -x

source stop-client.sh

sleep 1

cd ~/srds/sgx-srdsserver
${SGX_SDK:="/opt/intel/sgxsdk"}
source $SGX_SDK/environment
make clean
make
./SGXSRDSServerApp 8888 1 &

