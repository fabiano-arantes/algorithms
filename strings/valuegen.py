#!/usr/bin/python

import random
import sys

for x in range(0, int(sys.argv[1])):
	print ''.join(random.choice('0123456789ABCDEF') for i in range(16))
