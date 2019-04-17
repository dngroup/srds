#!/bin/bash

set -x

source exp-nucs-addr.sh

scp -o ProxyCommand="ssh -W %h:%p pi@$raspi" -r nuc@"$nuc4":/home/nuc/wrk2/logs /home/simon/Documents/SRDS2018/github/sgx-srdsserver/results/

