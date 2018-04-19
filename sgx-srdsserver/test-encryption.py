#!/usr/bin/python3

import os

port = 8081
mode = 11 # 0: proxy, 1: tracker, 11: tracker encryption test
filename = './encryptedtext.out'
text = '12345678newline\n'
sleeptime = 3

def main():
	os.system('make clean && make')
	print('[TEST] Starting app on port ' + str(port) + ' with mode ' + str(mode))
	os.system('(sleep ' + str(sleeptime) + ' && curl -d "' + str(text) + '" -X GET http://localhost:' + str(port) + ') &')
	os.system('(sleep ' + str(sleeptime+1) + ' && curl -d "' + str(text) + '" -X GET http://localhost:' + str(port) + ') &')
	os.system('(sleep ' + str(sleeptime+2) + ' && curl -d "' + str(text) + '" -X GET http://localhost:' + str(port) + ') &')
	os.system('(sleep ' + str(sleeptime+3) + ' && curl -d "' + str(text) + '" -X GET http://localhost:' + str(port) + ') &')
	os.system('./SGXSRDSServerApp ' + str(port) + ' ' + str(mode))

if __name__ == "__main__":
	main()
