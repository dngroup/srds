#!/bin/bash

set -x

source exp-nucs-addr.sh

ssh -f pi@"$raspi" "killall node"

command="~/srds/sgx-srdsserver/stop-client.sh"

ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc2" "$command"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc3" "$command"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc4" "$command"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc5" "$command"

ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc11" "$command"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc12" "$command"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc13" "$command"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc14" "$command"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc15@"$nuc15" "$command"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc16@"$nuc16" "$command"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc17@"$nuc17" "$command"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc18@"$nuc18" "$command"

sleep 1

ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" nuc@"$nuc6" "killall start-tracker.sh"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc6" "$command"

sleep 1

ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" nuc@"$nuc1" "killall start-content-server.sh"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc1" "$command"

