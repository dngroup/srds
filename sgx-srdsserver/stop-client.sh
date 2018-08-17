#!/bin/bash

set -x

killall start-tracker.sh
killall start-content-server.sh
killall firefox
killall chromium-browser
killall chrome
killall google-chrome
killall SGXSRDSServerApp
killall firefox
killall chromium-browser
killall chrome
killall google-chrome
killall SGXSRDSServerApp
killall node
killall http-server
killall gdb
docker stop $(docker ps -a -q)
docker rm $(docker ps -a -q)

