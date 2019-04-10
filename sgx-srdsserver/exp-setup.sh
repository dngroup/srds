#!/bin/bash

set -x

source exp-nucs-addr.sh
source exp-stop.sh

#ssh pi@"$raspi" "sudo dhcpcd --renew eth0"

sleep 20

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

ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc11" 'docker run --cap-add=ALL -d --net=host --name=tc-rest-controller mlacaud/tc-rest-controller -iu eno1'
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc12" 'docker run --cap-add=ALL -d --net=host --name=tc-rest-controller mlacaud/tc-rest-controller -iu eno1'
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc13" 'docker run --cap-add=ALL -d --net=host --name=tc-rest-controller mlacaud/tc-rest-controller -iu eno1'
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc14" 'docker run --cap-add=ALL -d --net=host --name=tc-rest-controller mlacaud/tc-rest-controller -iu eno1'
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc15@"$nuc15" 'docker run --cap-add=ALL -d --net=host --name=tc-rest-controller mlacaud/tc-rest-controller -iu eno1'
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc16@"$nuc16" 'docker run --cap-add=ALL -d --net=host --name=tc-rest-controller mlacaud/tc-rest-controller -iu eno1'
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc17@"$nuc17" 'docker run --cap-add=ALL -d --net=host --name=tc-rest-controller mlacaud/tc-rest-controller -iu eno1'
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc18@"$nuc18" 'docker run --cap-add=ALL -d --net=host --name=tc-rest-controller mlacaud/tc-rest-controller -iu eno1'

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

ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc2" "$command1 client1"
#ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc2" "$command2 $nuc2 1"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc2" 'for i in {1..17}; do curl --data "srds,adresse,$i" 192.168.1.120:8888; done'
sleep 10
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc3" "$command1 client2"
#ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc3" "$command2 $nuc3 1"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc3" 'for i in {1..17}; do curl --data "srds,adresse,$i" 192.168.1.120:8888; done'
sleep 10
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc4" "$command1 client3"
#ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc4" "$command2 $nuc4 1"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc4" 'for i in {1..17}; do curl --data "srds,adresse,$i" 192.168.1.120:8888; done'
sleep 10
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc5" "$command1 client4"
#ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc5" "$command2 $nuc5 1"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc5" 'for i in {1..17}; do curl --data "srds,adresse,$i" 192.168.1.120:8888; done'
sleep 10
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc11" "$command1 client5"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc12" "$command1 client8"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc13" "$command1 client9"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc14" "$command1 client10"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc16@"$nuc16" "$command1 client6"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc17@"$nuc17" "$command1 client7"

ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc15@"$nuc15" "$command1 client11"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc18@"$nuc18" "$command1 client12"

sleep 5

sleep 1000

