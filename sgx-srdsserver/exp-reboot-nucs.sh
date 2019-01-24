#!/bin/bash

set -x

source exp-stop.sh

sleep 10

command="sudo reboot"

ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -t nuc@"$nuc1" "$command"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -t nuc@"$nuc2" "$command"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -t nuc@"$nuc3" "$command"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -t nuc@"$nuc4" "$command"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -t nuc@"$nuc5" "$command"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -t nuc@"$nuc6" "$command"

ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -t nuc@"$nuc11" "$command"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -t nuc@"$nuc12" "$command"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -t nuc@"$nuc13" "$command"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -t nuc@"$nuc14" "$command"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -t nuc15@"$nuc15" "$command"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -t nuc16@"$nuc16" "$command"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -t nuc17@"$nuc17" "$command"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -t nuc18@"$nuc18" "$command"

