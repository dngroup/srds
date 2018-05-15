#!/bin/bash

set -x

cd ~/srds/sgx-srdsserver
source ~/srds/sgx-srdsserver/stop-client.sh

sleep 1

cd ~/content
docker run -d -p 8080:8080 -v $(pwd):/src/main/ressources msstream/server_srds

sleep 1

cd ~/player
http-server --cors -p 8082 &

sleep 1

cd ~/srds/sgx-srdsserver
${SGX_SDK:="/opt/intel/sgxsdk"}
source $SGX_SDK/environment
make clean
make
./SGXSRDSServerApp 8081 0 &

sleep 1

firefox -headless http://localhost:8082/samples/dash-if-reference-player/index.html

