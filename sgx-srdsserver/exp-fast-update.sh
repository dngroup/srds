#!/bin/bash

set -x

source exp-nucs-addr.sh

command="cd ~/srds/sgx-srdsserver/ && git pull && cd ~/srds/sgx-srdsserver/ && git pull && cd ~/player && git pull && git checkout dev_srds && grunt"

ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc1" "$command"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc2" "$command"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc3" "$command"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc4" "$command"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc5" "$command"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc6" "$command"

ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc11" "$command"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc12" "$command"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc13" "$command"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc14" "$command"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc15@"$nuc15" "$command"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc16@"$nuc16" "$command"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc17@"$nuc17" "$command"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc18@"$nuc18" "$command"

