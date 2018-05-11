#!/bin/bash

ssh nuc@147.210.129.172 '~/srds/sgx-srdsserver/stop-client.sh &'
ssh nuc@147.210.129.119 '~/srds/sgx-srdsserver/stop-client.sh &'
ssh nuc@147.210.128.61 '~/srds/sgx-srdsserver/stop-client.sh &'
ssh nuc@147.210.129.156 '~/srds/sgx-srdsserver/stop-client.sh &'
ssh nuc@147.210.128.135 '~/srds/sgx-srdsserver/stop-client.sh &'

sleep 1

ssh nuc@147.210.129.241 '~/srds/sgx-srdsserver/stop-client.sh &'

sleep 1

source stop-client.sh

