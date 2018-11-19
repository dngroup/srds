#!/bin/bash

set -x

cd ~/content
docker run -d -p 8080:8080 -v ~/content:/src/main/ressources msstream/server_srds

sleep 1

cd ~/srds/sgx-srdsserver
source /opt/intel/sgxsdk/environment
make clean
make

#gdb -ex run -ex stop --args ./SGXSRDSServerApp 8081 0

./SGXSRDSServerApp 8081 0

while sleep 1; do ./SGXSRDSServerApp 8081 0; done

