#!/usr/bin/python

import sys
import numpy
import csv


if len(sys.argv) != 2:
  print "invalid invocation one arg...  one arg only please"

inputFileName = sys.argv[1]

firstCol = 0
secondCol = 15 

latencies = []

with open(inputFileName) as csvFile:
  csvReader = csv.reader(csvFile, delimiter=',')
  for row in csvReader:
    ts1 = int(row[firstCol])
    ts2 = int(row[secondCol])
    diff = ts2 - ts1
    print ts1, ts2, diff


npNums = numpy.array(latencies)
pctBins = []
for pct in [99, 99.9, 99.99, 99.999, 99.9999]:
  pctBins.append((pct, numpy.percentile(npNums, pct)))

print "   mean =", npNums.mean()
print " median =", numpy.median(npNums)
for (pct, lat) in pctBins:
  print '  {0}%:\t{1}'.format(pct, lat)

print " 50%     ", numpy.percentile(npNums, 50)
print " stdDev =", npNums.std()
print "    min =", npNums.min()
print "    max =", npNums.max()
print "  count =", len(npNums)
#print "dropped =", dropped


#with open(inputFileName) as f:
#  for line in f:
#    line = line.rstrip("\n")
#    if len(line) >= 129:
#      try:
#        ts1 = int(line[0:14])
#        ts2 = int(line[114:128])
#        print line, (ts2-ts1)
#      except ValueError:
#        print line
#    else:
#      print line
#
#print inputFileName


