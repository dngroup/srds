#!/bin/bash

set -x

source exp-nucs-addr.sh

ssh -f pi@"$raspi" "killall java"

command="~/srds/sgx-srdsserver/stop-client.sh"

ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc2" "$command"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc3" "$command"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc4" "$command"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc5" "$command"

sleep 1

ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" nuc@"$nuc6" "killall start-tracker.sh"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc6" "$command"

sleep 1

ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" nuc@"$nuc1" "killall start-content-server.sh"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc1" "$command"

