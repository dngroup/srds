#!/bin/bash

set -x

cd ~

if [ -d "./Downloads/linux-sgx-driver/" ]; then
  
  cd ./Downloads/linux-sgx-driver/
  
else
  
  cd ./linux-sgx-driver/
  
fi

sudo chmod -R 777 .

git config user.email "toto@toto.toto"
git config user.name "Toto"

git stash
git pull
make clean
make
sudo mkdir -p "/lib/modules/"`uname -r`"/kernel/drivers/intel/sgx"
sudo cp isgx.ko "/lib/modules/"`uname -r`"/kernel/drivers/intel/sgx"
sudo sh -c "cat /etc/modules | grep -Fxq isgx || echo isgx >> /etc/modules"
sudo /sbin/depmod
sudo depmod
sudo /sbin/modprobe isgx
sudo service aesmd restart

