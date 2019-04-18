#!/bin/bash

set -x

source exp-nucs-addr.sh
source exp-stop.sh

sleep 20

ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc1" '~/srds/sgx-srdsserver/start-content-server.sh'

sleep 2

ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc6" '~/srds/sgx-srdsserver/start-tracker.sh'

sleep 5

ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc4" "~/srds/sgx-srdsserver/start-benchmark-noSGX.sh $1 $2 $3 $4 &> ~/wrk2/logs/$1-$2-$3-$4-noSGX.txt" # threads connections duration requests/s

sleep 20

sleep $3

