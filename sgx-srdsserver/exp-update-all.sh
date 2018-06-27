#!/bin/bash

set -x

source exp-nucs-addr.sh
source exp-stop.sh

sleep 10

#ssh -t <nuc>@<IP> 'sudo bash -c "echo \"*/10 * * * * <nuc> curl -X POST http://msstream.net:3000/nucX\" >> /etc/crontab"'

python exp-replace-addr.py --tracker "$nuc6" --mpd "$nuc1" --content "5GGKUZXWKE2LVA5VMXRA4"

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

command2="cd ~/Downloads/linux-sgx-driver/ && make clean && make && sudo mkdir -p \"/lib/modules/\"`uname -r`\"/kernel/drivers/intel/sgx\" && sudo cp isgx.ko \"/lib/modules/\"`uname -r`\"/kernel/drivers/intel/sgx\" && sudo sh -c \"cat /etc/modules | grep -Fxq isgx || echo isgx >> /etc/modules\" && sudo /sbin/depmod && sudo /sbin/modprobe isgx && sudo service aesmd restart"
command3="cd ~/linux-sgx-driver/ && make clean && make && sudo mkdir -p \"/lib/modules/\"`uname -r`\"/kernel/drivers/intel/sgx\" && sudo cp isgx.ko \"/lib/modules/\"`uname -r`\"/kernel/drivers/intel/sgx\" && sudo sh -c \"cat /etc/modules | grep -Fxq isgx || echo isgx >> /etc/modules\" && sudo /sbin/depmod && sudo /sbin/modprobe isgx && sudo service aesmd restart"

ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -t nuc@"$nuc1" "$command2"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -t nuc@"$nuc1" "$command3"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -t nuc@"$nuc2" "$command2"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -t nuc@"$nuc2" "$command3"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -t nuc@"$nuc3" "$command2"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -t nuc@"$nuc3" "$command3"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -t nuc@"$nuc4" "$command2"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -t nuc@"$nuc4" "$command3"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -t nuc@"$nuc5" "$command2"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -t nuc@"$nuc5" "$command3"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -t nuc@"$nuc6" "$command2"
ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" -t nuc@"$nuc6" "$command3"

sleep 10

