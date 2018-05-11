#!/bin/bash

source exp-stop.sh

sleep 30

ssh nuc@147.210.129.172 'cd ~/srds/sgx-srdsserver/ && git pull && cd ~/player && git pull && git checkout dev_srds && grunt &'
ssh nuc@147.210.129.119 'cd ~/srds/sgx-srdsserver/ && git pull && cd ~/player && git pull && git checkout dev_srds && grunt &'
ssh nuc@147.210.128.61 'cd ~/srds/sgx-srdsserver/ && git pull && cd ~/player && git pull && git checkout dev_srds && grunt &'
ssh nuc@147.210.129.156 'cd ~/srds/sgx-srdsserver/ && git pull && cd ~/player && git pull && git checkout dev_srds && grunt &'
ssh nuc@147.210.128.135 'cd ~/srds/sgx-srdsserver/ && git pull && cd ~/player && git pull && git checkout dev_srds && grunt &'

