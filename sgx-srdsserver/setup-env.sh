#!/bin/bash

cd ~
curl -sL https://deb.nodesource.com/setup_8.x | sudo -E bash -
sudo apt update
sudo apt install -y git firefox nodejs curl
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
make clean
make

