#!/bin/bash

set -x

cd ~/srds/sgx-srdsserver
source ~/srds/sgx-srdsserver/stop-client.sh

sleep 1

cd ~/srds/sgx-srdsserver
${SGX_SDK:="/opt/intel/sgxsdk"}
source $SGX_SDK/environment
make clean
make
./SGXSRDSServerApp 8888 1 &

