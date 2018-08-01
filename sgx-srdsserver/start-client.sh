#!/bin/bash

set -x

cd ~/srds/sgx-srdsserver
source ~/srds/sgx-srdsserver/stop-client.sh

sleep 1

cd ~/content
docker run -d -p 8080:8080 -v ~/content:/src/main/ressources msstream/server_srds

sleep 1

cd ~/player
git checkout dev_srds
http-server --cors -p 8082 &

sleep 1

cd ~/srds/sgx-srdsserver
source /opt/intel/sgxsdk/environment
make clean
make
./SGXSRDSServerApp 8081 0 &

sleep 10

chromium-browser --headless http://localhost:8082/samples/dash-if-reference-player/index.html

