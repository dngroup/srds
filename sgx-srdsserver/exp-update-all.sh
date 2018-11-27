#!/bin/bash

set -x

source exp-nucs-addr.sh
source exp-stop.sh

sleep 10

#ssh -t <nuc>@<IP> 'sudo bash -c "echo \"*/10 * * * * <nuc> curl -X POST http://msstream.net:3000/nucX\" >> /etc/crontab"'

# python exp-replace-addr.py --tracker "$nuc6" --mpd "192.168.1.109:8080" --content "192.168.1.109:8080" --opt1 192.168.1.104:8081 --opt2 192.168.1.107:8081 --opt3 192.168.1.118:8081 --opt4 192.168.1.110:8081 --opt5 192.168.1.108:8081

python exp-replace-addr.py --tracker "$nuc6" --mpd "$nuc1" --content "5GGKUZXWKE2LVA5VMXRA5JAWJKHQE" --opt1 5GGKUZXWKE2LVA5VMXRAHJAWJKHQE --opt2 5GGKUZXWKE2LVA5VMXRABJAWJKHQE --opt3 5GGKUZXWKE2LVA5VMXRQ7JAWJKHQE --opt4 5GGKUZXWKE2LVA5VMXRQPJAWJKHQE --opt5 5GGKUZXWKE2LVA5VMXRA7JAWJKHQE

scp /home/simon/Downloads/srds/mpd.mpd pi@"$raspi":~/content/srds/mpd.mpd

sleep 2

scp pi@"$raspi":~/content/srds/mpd.mpd nuc@"$nuc1":~/content/srds/mpd.mpd
scp pi@"$raspi":~/content/srds/mpd.mpd nuc@"$nuc2":~/content/srds/mpd.mpd
scp pi@"$raspi":~/content/srds/mpd.mpd nuc@"$nuc3":~/content/srds/mpd.mpd
scp pi@"$raspi":~/content/srds/mpd.mpd nuc@"$nuc4":~/content/srds/mpd.mpd
scp pi@"$raspi":~/content/srds/mpd.mpd nuc@"$nuc5":~/content/srds/mpd.mpd
scp pi@"$raspi":~/content/srds/mpd.mpd nuc@"$nuc6":~/content/srds/mpd.mpd

sleep 1

git commit SGXSRDSEnclave/SGXSRDSEnclave.cpp -m "auto-addr"

sleep 1

git push

sleep 1

command="cd ~/srds/sgx-srdsserver/ && git pull && cd ~/player && git pull && git checkout dev_srds && grunt"

ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc1" "$command"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc2" "$command"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc3" "$command"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc4" "$command"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc5" "$command"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc6" "$command"

sleep 60

command="source ~/srds/sgx-srdsserver/update-sgx.sh"

ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -t nuc@"$nuc1" "$command"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -t nuc@"$nuc2" "$command"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -t nuc@"$nuc3" "$command"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -t nuc@"$nuc4" "$command"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -t nuc@"$nuc5" "$command"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -t nuc@"$nuc6" "$command"

sleep 10

