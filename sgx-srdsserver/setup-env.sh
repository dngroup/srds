#!/bin/bash

git config --global credential.helper store

sudo apt update
sudo apt install -y git
sudo apt install -y firefox 
sudo apt install -y nodejs
sudo apt install -y nodejs
sudo apt install -y curl

cd ~
curl -sL https://deb.nodesource.com/setup_8.x | sudo -E bash -
sudo npm -g install grunt
sudo npm -g install grunt-cli
sudo npm -g install http-server

cd ~
curl -fsSL get.docker.com -o get-docker.sh && sh get-docker.sh
sudo groupadd docker
sudo usermod -aG docker $USER

cd ~
mkdir content

cd ~
git clone https://github.com/dngroup/msstream-player.git player
cd ~/player
git checkout dev_srds
npm install
grunt

cd ~
git clone https://github.com/dngroup/srds.git srds
cd ~/srds/sgx-srdsserver
${SGX_SDK:="/opt/intel/sgxsdk"}
source $SGX_SDK/environment
make clean
make

