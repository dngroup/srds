#!/bin/bash

set -x

killall firefox chromium-browser chrome google-chrome SGXSRDSServerApp firefox chromium-browser chrome google-chrome SGXSRDSServerApp node http-server gdb python3 bash
docker rm $(docker stop $(docker ps -a -q))

