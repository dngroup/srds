#!/bin/bash

set -x

source exp-nucs-addr.sh
source exp-stop.sh

sleep 10

ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc1" '~/srds/sgx-srdsserver/start-content-server.sh'

sleep 1

ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc6" '~/srds/sgx-srdsserver/start-tracker.sh'

sleep 1

command="~/srds/sgx-srdsserver/start-client.sh"

ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc2" "$command"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc3" "$command"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc4" "$command"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc5" "$command"

