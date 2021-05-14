#! /usr/bin/python3

import matplotlib
import matplotlib.pyplot as plt
import sys
import csv

matplotlib.use('TkAgg')

d_offset = []
time = []

with open(sys.argv[1]) as f:
    reader = csv.DictReader(f)
    for row in reader:
        d_offset.append(int(row['d_offset']) / 1e6)
        time.append(int(row['ns']) / 1e6)

    print(len(d_offset), len(time))
    plt.scatter(d_offset, time, s=1)
    plt.xlim(min(d_offset), max(d_offset))
    plt.ylim(0, max(time))
    plt.xlabel('d_offset [MB]')
    plt.ylabel('time [ms]')
    plt.show()
