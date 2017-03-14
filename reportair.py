#!/usr/bin/env python3
# reportair.py
# take report from airport extreme & give me map of reserved IPs
# as well as a report with spaces for open IPs

# 2017 03 13 SeJ init


###
### imports and parse args
###

### imports
import os
import argparse
import json
import xmltodict
from pprint import pprint
from operator import itemgetter, attrgetter
import socket

# parsing
parser = argparse.ArgumentParser(description='Reporting of reserved IPs')
parser.add_argument('file', nargs='?', type=argparse.FileType('r'),
                    help='input file', default='airport.baseconfig')
parser.add_argument('-t', '--test', action='store_true',
                    help='offline testing')

args = parser.parse_args()
userdir = os.path.expanduser("~")
print('userdir: ' + userdir)
print('input file: ' + args.file.name)


###
### variables
###


###
### defined functions
###


###
### main loop
###

def main():
        raw = args.file.read()
        conv = xmltodict.parse(raw)
        reserve = conv['plist']['dict']['dict'][1]['array']['dict']
        iptable = []
        for x in reserve:
                iptable.append([x['string'][1], x['string'][0], x['string'][2]])
        sortediptable = sorted(iptable, key=lambda
                             item:socket.inet_aton(item[0]))
        pprint(sortediptable)
        with open('airport.conv', 'w') as fout:
                pprint(sortediptable, stream = fout)
        x1 = socket.inet_aton(sortediptable[0][0])[-1]

        x2 = socket.inet_aton(sortediptable[-1][0])[-1]
        sptable = []
        for x in range(0, 256):
                ip = ['10.0.1.'+str(x), '-', '-']
                fail = True
                for r in range(len(sortediptable)):
                        if sortediptable[r][0] == ip[0]:
                                sptable.append(sortediptable[r])
                                fail = False
                                break
                if fail:  sptable.append(ip)
        pprint(sptable)
        with open('airport.full', 'w') as fout:
                pprint(sptable, stream = fout)
        return




if __name__== '__main__':
    main()
    exit()

