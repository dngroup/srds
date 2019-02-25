#!/bin/bash

set -x

source exp-nucs-addr.sh
source exp-stop.sh

#ssh pi@"$raspi" "sudo dhcpcd --renew eth0"

sleep 10

#ssh -t <nuc>@<IP> 'sudo bash -c "echo \"*/10 * * * * <nuc> curl -X POST http://msstream.net:3000/nucX\" >> /etc/crontab"'

#python exp-replace-addr.py --tracker "$nuc6" --mpd "$nuc1" --content "192.168.1.109:8080" --opt1 192.168.1.104:8080 --opt2 192.168.1.107:8080 --opt3 192.168.1.118:8080 --opt5 192.168.1.110:8080 #--opt4 192.168.1.108:8080

python exp-replace-addr.py --tracker "$nuc6" --mpd "$nuc1" --content "5GGKUZXWKE2LVA5VMXRA5JAWJKHQE" --opt1 5GGKUZXWKE2LVA5VMXRAHJAWJKHQE --opt2 5GGKUZXWKE2LVA5VMXRABJAWJKHQE --opt3 5GGKUZXWKE2LVA5VMXRQ7JAWJKHQE --opt5 5GGKUZXWKE2LVA5VMXRQPJAWJKHQE #--opt4 5GGKUZXWKE2LVA5VMXRA7JAWJKHQE

scp /home/simon/Downloads/srds/mpd.mpd pi@"$raspi":~/content/srds/mpd.mpd

sleep 1

scp pi@"$raspi":~/content/srds/mpd.mpd nuc@"$nuc1":~/content/srds/mpd.mpd
scp pi@"$raspi":~/content/srds/mpd.mpd nuc@"$nuc2":~/content/srds/mpd.mpd
scp pi@"$raspi":~/content/srds/mpd.mpd nuc@"$nuc3":~/content/srds/mpd.mpd
scp pi@"$raspi":~/content/srds/mpd.mpd nuc@"$nuc4":~/content/srds/mpd.mpd
scp pi@"$raspi":~/content/srds/mpd.mpd nuc@"$nuc5":~/content/srds/mpd.mpd
scp pi@"$raspi":~/content/srds/mpd.mpd nuc@"$nuc6":~/content/srds/mpd.mpd

scp pi@"$raspi":~/content/srds/mpd.mpd nuc@"$nuc11":~/content/srds/mpd.mpd
scp pi@"$raspi":~/content/srds/mpd.mpd nuc@"$nuc12":~/content/srds/mpd.mpd
scp pi@"$raspi":~/content/srds/mpd.mpd nuc@"$nuc13":~/content/srds/mpd.mpd
scp pi@"$raspi":~/content/srds/mpd.mpd nuc@"$nuc14":~/content/srds/mpd.mpd
scp pi@"$raspi":~/content/srds/mpd.mpd nuc15@"$nuc15":~/content/srds/mpd.mpd
scp pi@"$raspi":~/content/srds/mpd.mpd nuc16@"$nuc16":~/content/srds/mpd.mpd
scp pi@"$raspi":~/content/srds/mpd.mpd nuc17@"$nuc17":~/content/srds/mpd.mpd
scp pi@"$raspi":~/content/srds/mpd.mpd nuc18@"$nuc18":~/content/srds/mpd.mpd

sleep 1

cd /home/simon/Documents/SRDS2018/github/sgx-srdsserver/
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

ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc11" "$command"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc12" "$command"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc13" "$command"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc@"$nuc14" "$command"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc15@"$nuc15" "$command"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc16@"$nuc16" "$command"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc17@"$nuc17" "$command"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -f nuc18@"$nuc18" "$command"

sleep 60

command="sudo systemctl daemon-reload && sudo service docker restart && source ~/srds/sgx-srdsserver/update-sgx.sh"

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

sleep 10

