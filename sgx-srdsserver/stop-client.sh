#!/bin/bash

set -x

killall firefox chromium-browser chrome google-chrome SGXSRDSServerApp firefox chromium-browser chrome google-chrome SGXSRDSServerApp node http-server gdb python3 bash
docker rm --force $(docker stop -t 5 $(docker ps -a -q))

