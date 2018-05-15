#!/bin/bash

set -x

ssh nuc@147.210.129.172 'screen -d -m ~/srds/sgx-srdsserver/stop-client.sh'
ssh nuc@147.210.129.119 'screen -d -m ~/srds/sgx-srdsserver/stop-client.sh'
ssh nuc@147.210.128.61 'screen -d -m ~/srds/sgx-srdsserver/stop-client.sh'
ssh nuc@147.210.129.156 'screen -d -m ~/srds/sgx-srdsserver/stop-client.sh'
ssh nuc@147.210.128.135 'screen -d -m ~/srds/sgx-srdsserver/stop-client.sh'

sleep 1

ssh nuc@147.210.129.241 'screen -d -m ~/srds/sgx-srdsserver/stop-client.sh'

sleep 1

source stop-client.sh

