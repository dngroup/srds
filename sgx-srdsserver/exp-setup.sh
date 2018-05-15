#!/bin/bash

set -x

source exp-stop.sh

sleep 10

docker run -d -p 8080:8080 -v /home/simon/Downloads:/src/main/ressources msstream/server_srds

sleep 1

cd /home/simon/Documents/SRDS2018/github/sgx-srdsserver/
${SGX_SDK:="/opt/intel/sgxsdk"}
source $SGX_SDK/environment
make clean
make
./SGXSRDSServerApp 8081 0 &

sleep 1

ssh -f nuc@147.210.129.241 '~/srds/sgx-srdsserver/start-tracker.sh'

sleep 1

ssh -f nuc@147.210.129.172 '~/srds/sgx-srdsserver/start-client.sh'
ssh -f nuc@147.210.129.119 '~/srds/sgx-srdsserver/start-client.sh'
ssh -f nuc@147.210.128.61 '~/srds/sgx-srdsserver/start-client.sh'
ssh -f nuc@147.210.129.156 '~/srds/sgx-srdsserver/start-client.sh'
ssh -f nuc@147.210.128.135 '~/srds/sgx-srdsserver/start-client.sh'

