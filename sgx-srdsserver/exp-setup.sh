#!/bin/bash

set -x

source exp-nucs-addr.sh
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

ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc6" '~/srds/sgx-srdsserver/start-tracker.sh'

sleep 1

command="~/srds/sgx-srdsserver/start-client.sh"

ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc1" "$command"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc2" "$command"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc3" "$command"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc4" "$command"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc5" "$command"

