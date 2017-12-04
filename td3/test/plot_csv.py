#!/usr/bin/env python
#-*- coding: utf-8 -*-
import sys
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import csv


def plot_csv(filename, save=False):
    # Extracting
    with open(filename) as csvfile:
        rows = list(csv.reader(csvfile))

    features = rows.pop(0)
    sizes = map(lambda x:int(x[0]), rows)
    rows = map(lambda x:x[1:], rows)
    
    # Plotting
    name = "Courbe de performance de l'algorithme sequentiel"

    t = np.arange(0.0, 2.0, 0.01)
    s1 = np.sin(2*np.pi*t)
    s2 = np.sin(4*np.pi*t)
    
    plt.figure(1)
    plt.subplot(111)
    #plt.semilogx(sizes, rows, 'b')
    plt.plot(sizes, rows, 'b')
    plt.xlabel('nombre de processus (np)')
    plt.ylabel('speed up')
    plt.title('courbe de speed up avec N=1000')    
    # plt.subplot(111)
    # plt.plot(sizes, [x*x/500000.0 for x in sizes], 'b')
    if save:
        fig.savefig(name);
    plt.show()

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python plot_csv.py filename.csv")
        exit(-1)
    plot_csv(sys.argv[1])
        
        
