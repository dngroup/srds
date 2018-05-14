#!/bin/bash

set -x

ssh nuc@147.210.129.172 'nohup ~/srds/sgx-srdsserver/stop-client.sh &'
ssh nuc@147.210.129.119 'nohup ~/srds/sgx-srdsserver/stop-client.sh &'
ssh nuc@147.210.128.61 'nohup ~/srds/sgx-srdsserver/stop-client.sh &'
ssh nuc@147.210.129.156 'nohup ~/srds/sgx-srdsserver/stop-client.sh &'
ssh nuc@147.210.128.135 'nohup ~/srds/sgx-srdsserver/stop-client.sh &'

sleep 1

ssh nuc@147.210.129.241 'nohup ~/srds/sgx-srdsserver/stop-client.sh &'

sleep 1

source stop-client.sh

