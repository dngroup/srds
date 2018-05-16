#!/bin/bash

set -x

cd ~/srds/sgx-srdsserver
source ~/srds/sgx-srdsserver/stop-client.sh

sleep 1

cd ~/srds/sgx-srdsserver
source /opt/intel/sgxsdk/environment
make clean
make
./SGXSRDSServerApp 8888 1 &

