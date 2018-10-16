#!/bin/bash

set -x

source exp-nucs-addr.sh
source exp-stop.sh

sleep 20

ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc1" '~/srds/sgx-srdsserver/start-content-server.sh'

sleep 2

ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc6" '~/srds/sgx-srdsserver/start-tracker.sh'

sleep 5

#command="~/srds/sgx-srdsserver/start-client.sh"

#ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc2" "$command"
#sleep 2
#ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc3" "$command"
#sleep 2
#ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc5" "$command"
#sleep 2
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" nuc@"$nuc4" "~/srds/sgx-srdsserver/start-benchmark.sh $1 $2 $3 $4 &> ~/wrk2/logs/$1-$2-$3-$4-new3.txt" # threads connections duration requests/s

