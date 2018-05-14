#!/bin/bash

set -x

source exp-stop.sh

sleep 10

ssh nuc@147.210.129.172 'cd ~/srds/sgx-srdsserver/ && git pull && cd ~/player && git pull && git checkout dev_srds && grunt &'
ssh nuc@147.210.129.119 'cd ~/srds/sgx-srdsserver/ && git pull && cd ~/player && git pull && git checkout dev_srds && grunt &'
ssh nuc@147.210.128.61 'cd ~/srds/sgx-srdsserver/ && git pull && cd ~/player && git pull && git checkout dev_srds && grunt &'
ssh nuc@147.210.129.156 'cd ~/srds/sgx-srdsserver/ && git pull && cd ~/player && git pull && git checkout dev_srds && grunt &'
ssh nuc@147.210.128.135 'cd ~/srds/sgx-srdsserver/ && git pull && cd ~/player && git pull && git checkout dev_srds && grunt &'
ssh nuc@147.210.129.241 'cd ~/srds/sgx-srdsserver/ && git pull && cd ~/player && git pull && git checkout dev_srds && grunt &'

sleep 10

