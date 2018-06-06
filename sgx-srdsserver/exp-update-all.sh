#!/bin/bash

set -x

source exp-nucs-addr.sh
source exp-stop.sh

sleep 10

#ssh -t <nuc>@<IP> 'sudo bash -c "echo \"*/10 * * * * <nuc> curl -X POST http://msstream.net:3000/nucX\" >> /etc/crontab"'

command="'cd ~/srds/sgx-srdsserver/ && git pull && cd ~/player && git pull && git checkout dev_srds && grunt'"

ssh -f nuc@"$nuc1" $command
ssh -f nuc@"$nuc2" $command
ssh -f nuc@"$nuc3" $command
ssh -f nuc@"$nuc4" $command
ssh -f nuc@"$nuc5" $command
ssh -f nuc@"$nuc6" $command

command2="'cd ~/Downloads/linux-sgx-driver/ && make clean && make && sudo mkdir -p \"/lib/modules/\"`uname -r`\"/kernel/drivers/intel/sgx\" && sudo cp isgx.ko \"/lib/modules/\"`uname -r`\"/kernel/drivers/intel/sgx\" && sudo sh -c \"cat /etc/modules | grep -Fxq isgx || echo isgx >> /etc/modules\" && sudo /sbin/depmod && sudo /sbin/modprobe isgx && sudo service aesmd restart'"
command3="'cd ~/linux-sgx-driver/ && make clean && make && sudo mkdir -p \"/lib/modules/\"`uname -r`\"/kernel/drivers/intel/sgx\" && sudo cp isgx.ko \"/lib/modules/\"`uname -r`\"/kernel/drivers/intel/sgx\" && sudo sh -c \"cat /etc/modules | grep -Fxq isgx || echo isgx >> /etc/modules\" && sudo /sbin/depmod && sudo /sbin/modprobe isgx && sudo service aesmd restart'"

ssh -t nuc@"$nuc1" $command2
ssh -t nuc@"$nuc1" $command3
ssh -t nuc@"$nuc2" $command2
ssh -t nuc@"$nuc2" $command3
ssh -t nuc@"$nuc3" $command2
ssh -t nuc@"$nuc3" $command3
ssh -t nuc@"$nuc4" $command2
ssh -t nuc@"$nuc4" $command3
ssh -t nuc@"$nuc5" $command2
ssh -t nuc@"$nuc5" $command3
ssh -t nuc@"$nuc6" $command2
ssh -t nuc@"$nuc6" $command3

sleep 10

