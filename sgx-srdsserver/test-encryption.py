#!/usr/bin/python3

import os

port = 8081
mode = 1 # 0: proxy, 1: tracker
filename = './encryptedtext.out'

def main():
	os.system('make clean && make')
	print('[TEST] Starting app on port ' + str(port) + ' with mode ' + str(mode))
	os.system('./SGXSRDSServerApp ' + str(port) + ' ' + str(mode))

if __name__ == "__main__":
	main()
