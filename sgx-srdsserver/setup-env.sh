#!/bin/bash

cd ~
git clone https://github.com/dngroup/srds.git srds
cd srds
git pull
cd sgx-srdsserver
make clean
make

