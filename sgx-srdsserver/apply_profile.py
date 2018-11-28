#!/usr/bin/python3

import csv
import os
import time
import sys

addr = sys.argv[1]
profile = sys.argv[2]

with open('profiles.csv', newline='') as csvfile:
    os.system('curl -s -S -X POST ' + addr + ':9010/api/upload/limit/10000')
    os.system('curl -s -S -X POST ' + addr + ':9010/api/download/limit/10000')
    reader = csv.DictReader(csvfile)
    profiles = list(reader)
    for value in profiles:
    	if int(value['Profile']) == int(profile):
        	os.system('curl -s -S -X PUT ' + addr + ':9010/api/download/limit/' + str(int(value['Bandwidth'])))
        	os.system('curl -s -S -H \'Content-Type: application/json\' -X PUT -d \'{\"delay\":\"' + str(int(value['RTT'])/2) + '\", \"loss\":\"' + str(float(value['Loss'])) + '\"}\' ' + addr + ':9010/api/download/netem')
        	time.sleep(10)

