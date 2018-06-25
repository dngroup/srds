#!/usr/bin/python3

import os
import sys
import fileinput

def main():
	for line in fileinput.input('./SGXSRDSEnclave/SGXSRDSEnclave.cpp', inplace=True):
		if line.strip().startswith('const std::string trackerAddr("'):
			line = 'const std::string trackerAddr("' + str('TRACKER') + ':8888");\n'
		if line.strip().startswith('const std::string mpdAddr("'):
			line = 'const std::string mpdAddr("' + str('MPD') + ':8081");\n'
		sys.stdout.write(line)

if __name__ == "__main__":
	main()

