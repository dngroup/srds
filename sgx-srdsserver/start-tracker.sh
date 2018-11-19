#!/bin/bash

set -x

cd ~/srds/sgx-srdsserver
source /opt/intel/sgxsdk/environment
make clean
make

#gdb -ex run -ex stop --args ./SGXSRDSServerApp 8888 1

./SGXSRDSServerApp 8888 1

while sleep 1; do ./SGXSRDSServerApp 8888 1; done

