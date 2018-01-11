import sys
import numpy as np
import matplotlib.pyplot as plt

def plot(u,v):
    fig = plt.figure()
    ax = fig.add_subplot(111, autoscale_on=True, xlim=(-50, 50), ylim=(-50, 50))
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
         print "USAGE : python encoder.py filename"
    static_visu(args[1])
