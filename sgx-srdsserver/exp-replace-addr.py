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
		if line.strip().startswith('<SegmentTemplate') and line.strip().endswith('dash.mp4"/>'):
			if args.content:
				line = '   <BaseURL>http://' + args.content + '/api/description/srds/</BaseURL>\n' + line
			if args.opt1:
				line = '   <BaseURL>http://' + args.opt1 + '/api/description/srds/</BaseURL>\n' + line
			if args.opt2:
				line = '   <BaseURL>http://' + args.opt2 + '/api/description/srds/</BaseURL>\n' + line
			if args.opt3:
				line = '   <BaseURL>http://' + args.opt3 + '/api/description/srds/</BaseURL>\n' + line
			if args.opt4:
				line = '   <BaseURL>http://' + args.opt4 + '/api/description/srds/</BaseURL>\n' + line
			if args.opt5:
				line = '   <BaseURL>http://' + args.opt5 + '/api/description/srds/</BaseURL>\n' + line
			if args.opt6:
				line = '   <BaseURL>http://' + args.opt6 + '/api/description/srds/</BaseURL>\n' + line
			sys.stdout.write(line)
		if not line.strip().startswith('<BaseURL>http://') and not line.strip().endswith('/api/description/srds/</BaseURL>'):
			sys.stdout.write(line)

if __name__ == "__main__":
	parser = argparse.ArgumentParser(description='Passing adresses')
	parser.add_argument('--tracker', type=str, help='Tracker address', required=True)
	parser.add_argument('--mpd', type=str, help='MPD address', required=True)
	parser.add_argument('--content', type=str, help='Content address', required=False)
	parser.add_argument('--opt1', type=str, help='Optional content', required=False)
	parser.add_argument('--opt2', type=str, help='Optional content', required=False)
	parser.add_argument('--opt3', type=str, help='Optional content', required=False)
	parser.add_argument('--opt4', type=str, help='Optional content', required=False)
	parser.add_argument('--opt5', type=str, help='Optional content', required=False)
	parser.add_argument('--opt6', type=str, help='Optional content', required=False)
	args = parser.parse_args() 
	main(args)

