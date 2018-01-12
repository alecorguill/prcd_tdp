from math import sqrt
import numpy as np
import sys
import csv

EPS = 1e-10

def rel_mean_error(x,y):
    return np.linalg.norm(x-y)/np.linalg.norm(x)

def to_float(x):
    try:
        return float(x.translate(None,'\x00'))
    except:
        pass
def test_mean_error(f1,f2):
    print "Test mean error"
    rows1,rows2 = [],[]
    with open(f1) as fp:
        for line in fp:
            meas = map(lambda x: to_float(x), line.split(','))
            rows1 += [meas]
    
    with open(f2) as fp:
        for line in fp:
            meas = map(lambda x: to_float(x), line.split(','))
            rows2 += [meas]

    if(len(rows1) != len(rows2)):
        print "Different size"
        return
    sum = 0
    for i in range(len(rows1)):
        rows1[i] = [float(x) for x in rows1[i]]
        rows2[i] = [float(x) for x in rows2[i]]
        sum += rel_mean_error(np.array(rows1[i]),np.array(rows2[i]))
    print float(sum)/len(rows1)
        
    print "OK"

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python tests.py force_fileA force_fileB")
        exit(-1)
    test_mean_error(sys.argv[1],sys.argv[2])
    
