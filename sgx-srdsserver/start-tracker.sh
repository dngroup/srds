#!/bin/bash

set -x

cd ~/srds/sgx-srdsserver
source ~/srds/sgx-srdsserver/stop-client.sh

sleep 1

cd ~/srds/sgx-srdsserver
source /opt/intel/sgxsdk/environment
make clean
make

#gdb -ex run -ex stop --args ./SGXSRDSServerApp 8888 1
while sleep 0.1; do ./SGXSRDSServerApp 8888 1; done

