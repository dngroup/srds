#!/bin/bash

set -x

source exp-nucs-addr.sh

command="'~/srds/sgx-srdsserver/stop-client.sh'"

ssh -f nuc@"$nuc1" $command
ssh -f nuc@"$nuc2" $command
ssh -f nuc@"$nuc3" $command
ssh -f nuc@"$nuc4" $command
ssh -f nuc@"$nuc5" $command

sleep 1

ssh -f nuc@"$nuc6" $command

sleep 1

source stop-client.sh

