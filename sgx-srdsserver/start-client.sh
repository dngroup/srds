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

#Xvfb :99 &

#sleep 2

#DISPLAY=:99.0

#sleep 2

chromium-browser --headless --remote-debugging-port=7777 --ignore-autoplay-restrictions --no-user-gesture-required --disable-default-apps --no-first-run --disk-cache-dir=/dev/null --disk-cache-size=1 http://localhost:8082/samples/dash-if-reference-player/index.html

