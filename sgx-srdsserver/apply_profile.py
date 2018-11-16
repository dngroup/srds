#!/usr/bin/python3

import csv
import os
import time
import sys

addr = sys.argv[1]

with open('profiles.csv', newline='') as csvfile:
    os.system('curl -X POST ' + addr + ':9000/api/upload/limit/1000000')
    os.system('curl -X POST ' + addr + ':9000/api/download/limit/1000000')
    reader = csv.DictReader(csvfile)
    profiles = list(reader)
    for value in profiles:
        os.system('curl -X PUT ' + addr + ':9000/api/upload/limit/' + str(int(value['Bandwidth'])))
        os.system('curl -X PUT ' + addr + ':9000/api/download/limit/' + str(int(value['Bandwidth'])))
        os.system('curl -H \'Content-Type: application/json\' -X PUT -d \'{\"delay\":\"' + str(int(value['RTT'])/2) + '\", \"loss\":\"' + str(float(value['Loss'])) + '\"}\' ' + addr + ':9000/api/upload/netem')
        os.system('curl -H \'Content-Type: application/json\' -X PUT -d \'{\"delay\":\"' + str(int(value['RTT'])/2) + '\", \"loss\":\"' + str(float(value['Loss'])) + '\"}\' ' + addr + ':9000/api/download/netem')
        time.sleep(2)
