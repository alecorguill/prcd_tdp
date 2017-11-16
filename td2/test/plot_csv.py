#!/usr/bin/env python
#-*- coding: utf-8 -*-
import sys
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import csv


def plot_csv(filename, name, xlabel=None, ylabel=None, save=False):
    # Extracting
    with open(filename) as csvfile:
        rows = list(csv.reader(csvfile))

    features = rows.pop(0)
    sizes = map(lambda x:int(x[0]), rows)
    rows = map(lambda x:x[1:], rows)

    # Plotting
    name = filename.split('.')[0]
    fig, ax = plt.subplots()
    ax.plot(sizes, rows)
    ax.set(xlabel=xlabel,ylabel=ylabel,title=name) 
    if save:
        fig.savefig(name);
    plt.show()

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python plot_csv.py filename.csv name [xlabel,ylabel]")
        exit(-1)
    if len(sys.argv) == 3: 
        plot_csv(sys.argv[1],sys.argv[2])
    if len(sys.argv) == 4: 
        plot_csv(sys.argv[1],sys.argv[2],sys.argv[3])
    if len(sys.argv) == 5: 
        plot_csv(sys.argv[1],sys.argv[2],sys.argv[3],sys.argv[4])
  
        
