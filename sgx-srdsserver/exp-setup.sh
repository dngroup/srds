#!/bin/bash

set -x

source exp-nucs-addr.sh
source exp-stop.sh

scp /home/simon/Documents/SRDS2018/github/sgx-srdsserver/logserver.js pi@"$raspi":/home/pi/logserver.js
ssh -f pi@"$raspi" "cd /home/pi/ && npm i express cors body-parser fs-extra shelljs npm && node logserver.js"

sleep 40

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

command="~/srds/sgx-srdsserver/start-client.sh"

ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc2" "$command"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc2" "cd ~/srds/sgx-srdsserver && python3 apply_profile.py $nuc2 1"
sleep 10
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc3" "$command"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc3" "cd ~/srds/sgx-srdsserver && python3 apply_profile.py $nuc3 2"
sleep 10
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc4" "$command"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc4" "cd ~/srds/sgx-srdsserver && python3 apply_profile.py $nuc4 3"
sleep 10
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc5" "$command"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc5" "cd ~/srds/sgx-srdsserver && python3 apply_profile.py $nuc5 4"

sleep 5

sleep 1000

