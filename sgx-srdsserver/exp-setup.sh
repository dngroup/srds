#!/bin/bash

set -x

source exp-nucs-addr.sh
source exp-stop.sh

ssh pi@"$raspi" "sudo dhcpcd --renew eth0"

sleep 10

scp /home/simon/Documents/SRDS2018/github/sgx-srdsserver/logserver.js pi@"$raspi":/home/pi/logserver.js

sleep 1

ssh -f pi@"$raspi" "cd /home/pi/ && npm i express cors body-parser fs-extra shelljs os npm && node logserver.js"

sleep 100

ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc1" 'docker run --cap-add=ALL -d --net=host --name=tc-rest-controller mlacaud/tc-rest-controller -iu eno1'
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc2" 'docker run --cap-add=ALL -d --net=host --name=tc-rest-controller mlacaud/tc-rest-controller -iu eno1'
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc3" 'docker run --cap-add=ALL -d --net=host --name=tc-rest-controller mlacaud/tc-rest-controller -iu eno1'
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc4" 'docker run --cap-add=ALL -d --net=host --name=tc-rest-controller mlacaud/tc-rest-controller -iu eno1'
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc5" 'docker run --cap-add=ALL -d --net=host --name=tc-rest-controller mlacaud/tc-rest-controller -iu eno1'
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc6" 'docker run --cap-add=ALL -d --net=host --name=tc-rest-controller mlacaud/tc-rest-controller -iu eno1'

sleep 5

ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc1" '~/srds/sgx-srdsserver/start-content-server.sh'
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc6" '~/srds/sgx-srdsserver/start-tracker.sh'

sleep 5

ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" nuc@"$nuc1" "curl -s -S -X POST $nuc1:9010/api/upload/limit/10000"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" nuc@"$nuc1" "curl -s -S -X PUT $nuc1:9010/api/upload/limit/10000"

ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" nuc@"$nuc6" "curl -s -S -X POST $nuc6:9010/api/upload/limit/10000"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" nuc@"$nuc6" "curl -s -S -X PUT $nuc6:9010/api/upload/limit/10000"

sleep 5

command1="~/srds/sgx-srdsserver/start-client.sh"
command2="cd ~/srds/sgx-srdsserver && python3 apply_profile.py"

#ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc2" "$command1 client1"
#ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc2" "$command2 $nuc2 1"
#sleep 10
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc3" "$command1 client2"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc3" "$command2 $nuc3 2"
sleep 10
#ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc4" "$command1 client3"
#ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc4" "$command2 $nuc4 3"
sleep 10
#ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc5" "$command1 client4"
#ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc5" "$command2 $nuc5 4"

sleep 5

sleep 1000

