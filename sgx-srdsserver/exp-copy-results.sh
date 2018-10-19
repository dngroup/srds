#!/bin/bash

set -x

source exp-nucs-addr.sh

scp -o ProxyCommand="ssh -W %h:%p pi@$raspi" nuc@"$nuc4":/home/nuc/wrk2/logs/wrk.csv pi@"$raspi":~/wrk.csv && scp pi@"$raspi":~/wrk.csv /home/simon/Documents/SRDS2018/github/sgx-srdsserver/results/wrk.csv

