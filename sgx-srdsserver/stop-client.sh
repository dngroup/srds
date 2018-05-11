#!/bin/bash

killall firefox
killall SGXSRDSServerApp
killall node
killall http-server
docker stop $(docker ps -a -q)
docker rm $(docker ps -a -q)

