#!/usr/bin/python3

import os
import sys
import fileinput
import argparse

def main(args):
	for line in fileinput.input('./SGXSRDSEnclave/SGXSRDSEnclave.cpp', inplace=True):
		if line.strip().startswith('const std::string trackerAddr("') and line.strip().endswith(':8888");'):
			line = 'const std::string trackerAddr("' + args.tracker + ':8888");\n'
		if line.strip().startswith('const std::string mpdAddr("') and line.strip().endswith(':8081");'):
			line = 'const std::string mpdAddr("' + args.mpd + ':8081");\n'
		sys.stdout.write(line)

	for line in fileinput.input('/home/simon/Downloads/srds/mpd.mpd', inplace=True):
		if line.strip().startswith('<BaseURL>http://') and line.strip().endswith('/api/description/srds/</BaseURL>'):
			line = '<BaseURL>http://' + args.mpd + '/api/description/srds/</BaseURL>\n'
		sys.stdout.write(line)

if __name__ == "__main__":
	parser = argparse.ArgumentParser(description='Passing adresses')
	parser.add_argument('--tracker', type=str, help='Tracker address', required=True)
	parser.add_argument('--mpd', type=str, help='MPD address', required=True)
	args = parser.parse_args() 
	main(args)

