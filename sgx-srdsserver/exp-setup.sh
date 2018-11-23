#!/bin/bash

set -x

source exp-nucs-addr.sh
source exp-stop.sh

sleep 30

ssh -f pi@"$raspi" "cd /home/pi/ && java -jar logserver.jar"

ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc1" 'docker run --cap-add=ALL -d --net=host --name=tc-rest-controller mlacaud/tc-rest-controller -iu eno1'
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc2" 'docker run --cap-add=ALL -d --net=host --name=tc-rest-controller mlacaud/tc-rest-controller -iu eno1'
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc3" 'docker run --cap-add=ALL -d --net=host --name=tc-rest-controller mlacaud/tc-rest-controller -iu eno1'
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc4" 'docker run --cap-add=ALL -d --net=host --name=tc-rest-controller mlacaud/tc-rest-controller -iu eno1'
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc5" 'docker run --cap-add=ALL -d --net=host --name=tc-rest-controller mlacaud/tc-rest-controller -iu eno1'
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc6" 'docker run --cap-add=ALL -d --net=host --name=tc-rest-controller mlacaud/tc-rest-controller -iu eno1'

sleep 5

ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc1" '~/srds/sgx-srdsserver/start-content-server.sh'

sleep 2

ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc6" '~/srds/sgx-srdsserver/start-tracker.sh'

sleep 5

command="~/srds/sgx-srdsserver/start-client.sh"

ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc1" "cd ~/srds/sgx-srdsserver && python3 apply_profile.py $nuc1"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc2" "cd ~/srds/sgx-srdsserver && python3 apply_profile.py $nuc2"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc3" "cd ~/srds/sgx-srdsserver && python3 apply_profile.py $nuc3"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc4" "cd ~/srds/sgx-srdsserver && python3 apply_profile.py $nuc4"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc5" "cd ~/srds/sgx-srdsserver && python3 apply_profile.py $nuc5"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc6" "cd ~/srds/sgx-srdsserver && python3 apply_profile.py $nuc6"

ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc2" "$command"
sleep 2
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc3" "$command"
sleep 2
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc4" "$command"
sleep 2
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc5" "$command"

sleep 1000

