#!/bin/bash

git pull
make clean
make
./SGXSRDSServerApp 8081 0 &
firefox http://localhost:8082/samples/dash-if-reference-player/index.html
killall SGXSRDSServerApp
killall firefox

