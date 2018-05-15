#!/bin/bash

set -x

source exp-stop.sh

sleep 10

ssh nuc@147.210.129.172 'cd ~/srds/sgx-srdsserver/ && git pull && cd ~/player && git pull && git checkout dev_srds && grunt && disown -a &'
ssh nuc@147.210.129.119 'cd ~/srds/sgx-srdsserver/ && git pull && cd ~/player && git pull && git checkout dev_srds && grunt && disown -a &'
ssh nuc@147.210.128.61 'cd ~/srds/sgx-srdsserver/ && git pull && cd ~/player && git pull && git checkout dev_srds && grunt && disown -a &'
ssh nuc@147.210.129.156 'cd ~/srds/sgx-srdsserver/ && git pull && cd ~/player && git pull && git checkout dev_srds && grunt && disown -a &'
ssh nuc@147.210.128.135 'cd ~/srds/sgx-srdsserver/ && git pull && cd ~/player && git pull && git checkout dev_srds && grunt && disown -a &'
ssh nuc@147.210.129.241 'cd ~/srds/sgx-srdsserver/ && git pull && cd ~/player && git pull && git checkout dev_srds && grunt && disown -a &'

ssh -t nuc@147.210.129.172 'cd ~/Downloads/linux-sgx-driver/ && make clean && make && sudo mkdir -p "/lib/modules/"`uname -r`"/kernel/drivers/intel/sgx" && sudo cp isgx.ko "/lib/modules/"`uname -r`"/kernel/drivers/intel/sgx" && sudo sh -c "cat /etc/modules | grep -Fxq isgx || echo isgx >> /etc/modules" && sudo /sbin/depmod && sudo /sbin/modprobe isgx && sudo service aesmd restart'
ssh -t nuc@147.210.129.172 'cd ~/linux-sgx-driver/ && make clean && make && sudo mkdir -p "/lib/modules/"`uname -r`"/kernel/drivers/intel/sgx" && sudo cp isgx.ko "/lib/modules/"`uname -r`"/kernel/drivers/intel/sgx" && sudo sh -c "cat /etc/modules | grep -Fxq isgx || echo isgx >> /etc/modules" && sudo /sbin/depmod && sudo /sbin/modprobe isgx && sudo service aesmd restart'
ssh -t nuc@147.210.129.119 'cd ~/Downloads/linux-sgx-driver/ && make clean && make && sudo mkdir -p "/lib/modules/"`uname -r`"/kernel/drivers/intel/sgx" && sudo cp isgx.ko "/lib/modules/"`uname -r`"/kernel/drivers/intel/sgx" && sudo sh -c "cat /etc/modules | grep -Fxq isgx || echo isgx >> /etc/modules" && sudo /sbin/depmod && sudo /sbin/modprobe isgx && sudo service aesmd restart'
ssh -t nuc@147.210.129.119 'cd ~/linux-sgx-driver/ && make clean && make && sudo mkdir -p "/lib/modules/"`uname -r`"/kernel/drivers/intel/sgx" && sudo cp isgx.ko "/lib/modules/"`uname -r`"/kernel/drivers/intel/sgx" && sudo sh -c "cat /etc/modules | grep -Fxq isgx || echo isgx >> /etc/modules" && sudo /sbin/depmod && sudo /sbin/modprobe isgx && sudo service aesmd restart'
ssh -t nuc@147.210.128.61 'cd ~/Downloads/linux-sgx-driver/ && make clean && make && sudo mkdir -p "/lib/modules/"`uname -r`"/kernel/drivers/intel/sgx" && sudo cp isgx.ko "/lib/modules/"`uname -r`"/kernel/drivers/intel/sgx" && sudo sh -c "cat /etc/modules | grep -Fxq isgx || echo isgx >> /etc/modules" && sudo /sbin/depmod && sudo /sbin/modprobe isgx && sudo service aesmd restart'
ssh -t nuc@147.210.128.61 'cd ~/linux-sgx-driver/ && make clean && make && sudo mkdir -p "/lib/modules/"`uname -r`"/kernel/drivers/intel/sgx" && sudo cp isgx.ko "/lib/modules/"`uname -r`"/kernel/drivers/intel/sgx" && sudo sh -c "cat /etc/modules | grep -Fxq isgx || echo isgx >> /etc/modules" && sudo /sbin/depmod && sudo /sbin/modprobe isgx && sudo service aesmd restart'
ssh -t nuc@147.210.129.156 'cd ~/Downloads/linux-sgx-driver/ && make clean && make && sudo mkdir -p "/lib/modules/"`uname -r`"/kernel/drivers/intel/sgx" && sudo cp isgx.ko "/lib/modules/"`uname -r`"/kernel/drivers/intel/sgx" && sudo sh -c "cat /etc/modules | grep -Fxq isgx || echo isgx >> /etc/modules" && sudo /sbin/depmod && sudo /sbin/modprobe isgx && sudo service aesmd restart'
ssh -t nuc@147.210.129.156 'cd ~/linux-sgx-driver/ && make clean && make && sudo mkdir -p "/lib/modules/"`uname -r`"/kernel/drivers/intel/sgx" && sudo cp isgx.ko "/lib/modules/"`uname -r`"/kernel/drivers/intel/sgx" && sudo sh -c "cat /etc/modules | grep -Fxq isgx || echo isgx >> /etc/modules" && sudo /sbin/depmod && sudo /sbin/modprobe isgx && sudo service aesmd restart'
ssh -t nuc@147.210.128.135 'cd ~/Downloads/linux-sgx-driver/ && make clean && make && sudo mkdir -p "/lib/modules/"`uname -r`"/kernel/drivers/intel/sgx" && sudo cp isgx.ko "/lib/modules/"`uname -r`"/kernel/drivers/intel/sgx" && sudo sh -c "cat /etc/modules | grep -Fxq isgx || echo isgx >> /etc/modules" && sudo /sbin/depmod && sudo /sbin/modprobe isgx && sudo service aesmd restart'
ssh -t nuc@147.210.128.135 'cd ~/linux-sgx-driver/ && make clean && make && sudo mkdir -p "/lib/modules/"`uname -r`"/kernel/drivers/intel/sgx" && sudo cp isgx.ko "/lib/modules/"`uname -r`"/kernel/drivers/intel/sgx" && sudo sh -c "cat /etc/modules | grep -Fxq isgx || echo isgx >> /etc/modules" && sudo /sbin/depmod && sudo /sbin/modprobe isgx && sudo service aesmd restart'
ssh -t nuc@147.210.129.241 'cd ~/Downloads/linux-sgx-driver/ && make clean && make && sudo mkdir -p "/lib/modules/"`uname -r`"/kernel/drivers/intel/sgx" && sudo cp isgx.ko "/lib/modules/"`uname -r`"/kernel/drivers/intel/sgx" && sudo sh -c "cat /etc/modules | grep -Fxq isgx || echo isgx >> /etc/modules" && sudo /sbin/depmod && sudo /sbin/modprobe isgx && sudo service aesmd restart'
ssh -t nuc@147.210.129.241 'cd ~/linux-sgx-driver/ && make clean && make && sudo mkdir -p "/lib/modules/"`uname -r`"/kernel/drivers/intel/sgx" && sudo cp isgx.ko "/lib/modules/"`uname -r`"/kernel/drivers/intel/sgx" && sudo sh -c "cat /etc/modules | grep -Fxq isgx || echo isgx >> /etc/modules" && sudo /sbin/depmod && sudo /sbin/modprobe isgx && sudo service aesmd restart'

sleep 10

