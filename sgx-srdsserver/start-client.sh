#!/bin/bash

cd ~/content
docker stop $(docker ps -a -q)
docker rm $(docker ps -a -q)
sudo docker run -d -p 8080:8080 -v $(pwd):/src/main/ressources msstream/server_srds

cd ~/player
git pull
git checkout dev_srds
grunt
http-server --cors -p 8082 &

cd ~/srds/sgx-srdsserver
make clean
git pull
make
./SGXSRDSServerApp 8081 0 &

firefox http://localhost:8082/samples/dash-if-reference-player/index.html

killall firefox
killall SGXSRDSServerApp
killall http-server
docker stop $(docker ps -a -q)
docker rm $(docker ps -a -q)

