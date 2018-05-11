#!/bin/bash

git config --global credential.helper store

source stop-client.sh

cd ~/content
docker stop $(docker ps -a -q)
docker rm $(docker ps -a -q)
sudo docker run -d -p 8080:8080 -v $(pwd):/src/main/ressources msstream/server_srds

sleep 1

cd ~/player
killall node
killall http-server
git pull
git checkout dev_srds
grunt
http-server --cors -p 8082 &

sleep 1

cd ~/srds/sgx-srdsserver
${SGX_SDK:="/home/nuc/linux-sgx/linux/installer/bin/sgxsdk"}
source $SGX_SDK/environment
make clean
git pull
make
./SGXSRDSServerApp 8081 0 &

sleep 1

killall firefox

sleep 1

firefox http://localhost:8082/samples/dash-if-reference-player/index.html &

