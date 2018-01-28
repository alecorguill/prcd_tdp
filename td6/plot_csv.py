#!/usr/bin/env python
#-*- coding: utf-8 -*-
import sys
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import csv
import copy

def extract_csv(filename, save=False):
    # Extracting
    f = open(filename,'r')
    content = f.read().split("\n")
    sizes = [int(x.split(",")[0]) for x in content[2:-1]]
    sequentiel = float(content[1])
    openmp = [float(x.split(",")[1]) for x in content[2:-1]]
    pthread = [float(x.split(",")[2]) for x in content[2:-1]]
    return (sizes,sequentiel,openmp,pthread)
    
if __name__ == "__main__":
    plt.figure(1)
    plt.subplot(111)
    size,sequentiel,openmp,pthread = extract_csv("time.csv");
    save = sequentiel
    for i in range(len(openmp)):
        openmp[i] = save / openmp[i]
        pthread[i] = save / pthread[i]

    xy = [[],[]]
    for i in range(1,size[-1]):
        xy[0].append(i)
        xy[1].append(i)

    plt.plot(xy[0],xy[1],'r')
    plt.plot(size, openmp, 'b', label='openmp')
    plt.plot(size, pthread, 'g', label='pthread')
    plt.legend(loc=1, bbox_to_anchor=(1,0.87))

    plt.xlabel('nombre de processus')
    plt.ylabel('Speed up')    
    plt.title('Speedup')    
    plt.show()
