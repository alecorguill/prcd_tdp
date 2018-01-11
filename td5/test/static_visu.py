import sys
import numpy as np
import matplotlib.pyplot as plt

def plot(u,v):
    fig = plt.figure()
    ax = fig.add_subplot(111, autoscale_on=False, xlim=(0, 10), ylim=(0, 10))
    ax.grid()
    plt.scatter(u,v)
    plt.show()

def static_visu(filename):
    u, v = [], []
    with open(filename) as fp:
        fp.readline()
        for line in fp:
            rst = line.split()
            u.append(float(rst[1]))
            v.append(float(rst[2]))


    plot(u,v)
 
if __name__ == '__main__':
    args = sys.argv
    if len(args) != 2:
         print "Usage : python encoder.py filename"
    else:
        static_visu(args[1])
