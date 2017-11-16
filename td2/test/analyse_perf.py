#!/usr/bin/env python
#-*- coding: utf-8 -*-
## Ce fichier doit être appelé depuis le répertoire test

import os, sys
import matplotlib.pyplot as plt
import subprocess, time
import matplotlib.pyplot as plt
#import pandas as pd
import csv

np_default = 5
nb_part_default = 200
nb_proc_step = 2
size = 100

def save_csv(filename,x,y,columnx,columny):
    with open(filename, "wb") as csv_file:
        writer = csv.writer(csv_file, delimiter=',')
        writer.writerow([columnx,columny])
        for i in range(x):
            writer.writerow([str(x),str(y)])

def perf_seq(input, output, save_to_csv=False):
    time_seq = []
    os.system("make -C ../ clean")
    os.system("make -C ../ sequentiel")
    N = range(1,nb_part_default)
    # # Perf sequentiel
    for n in N:
        ##
        os.system("python generate_particules.py {} {}".format(input,n))
        start = time.time()
        os.system("../sequentiel {} {}".format(input,output))
        end = time.time()
        time_seq.append(end-start)
    if save_to_csv:
        save_csv('perf_seq',N,time_seq,'size','perf_seq')
    return time_seq

def speed_seq(input, output, save_to_csv=False):
    speed_seq = [] 

    os.system("make -C ../ clean")
    os.system("make -C ../ sequentiel")
   
    os.system("python generate_particules.py {} {}".format(input,nb_part_default))

    P = range(nb_proc_speedup)
    ## Speed up parallel
    for p in P:
    ##
        start = time.time()
        os.system("../sequentiel {} {}".format(input,output))
        end = time.time()
        speed_seq.append(end-start)
        time_seq.append(end-start)
    if save_to_csv:    
        save_csv('speed_seq',P,speed_seq,'size','speed_seq')
    return speed_seq

def perf_par(input, output, save_to_csv=False):
    time_par = []
    os.system("make -C ../ clean")
    os.system("make -C ../ parallel")
    N = range(np_default,nb_part_default,np_default) 
    
    # Perf parallel
    for n in N:
        ##
        os.system("python generate_particules.py {} {}".format(input,n))
        start = time.time()
        os.system("mpirun -mca pml ob1 -np {} ./parallel {} {}".format(np_default,input,output))
        end = time.time()
        time_par.append(end-start)
    if save_to_csv:
        save_csv('perf_par',N,time_par,'size','perf_par')
    return time_par

def speed_par():
    speed_par = []
    os.system("make -C ../ clean")
    os.system("make -C ../ parallel")

    ##Speed up    
    P = range(nb_proc_speedup)
    os.system("python generate_particules.py {} {}".format(input,nb_part_default))
    for p in P:
        ##
        start = time.time()
        os.system("mpirun -mca pml ob1 -np {} parallel {} {}".format(p,input,output))
        end = time.time()
        speed_par.append(end-start)
    if save_to_csv:
        save_csv('speed_par',P,speed_par,'size','speed_par')
  
    return speed_par


def perf(input, output):
   
    time_seq = perf_seq(input, output)
    time_par = perf_par(input, output)
    speed_seq = speed_seq(input, output)
    speed_par = speed_par(input, output)
    return time_seq,time_par,speed_seq,speed_par


def plot(x,y,name,xlabel=None,ylabel=None,label=None):
    # # Plotting
    fig, ax = plt.subplots()
    ax.set(xlabel=xlabel,ylabel=ylabel,title=name)
    ax.plot(x,y,label)
    
# the csv files need to contain only 2 columns
def plot_csv(filename, xlabel=None, label=None, ylabel=None, save=False):
    # Extracting
    with open(filename) as csvfile:
        rows = list(csv.reader(csvfile))

    features = rows.pop(0)
    sizes = map(lambda x:int(x[0]), rows)
    rows = map(lambda x:x[1:], rows)

    # Plotting
    name = filename.split('.')[0]
    fig, ax = plt.subplots()
    ax.plot(sizes, flops,label=label)
    ax.set(xlabel=xlabel,ylabel=ylabel,title=name) 
    if save:
        fig.savefig(name);


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python analyse_perf.py input output")
        exit(-1)
    
    # time_seq,time_par,speed_seq,speed_par = perf(sys.argv[1],sys.argv[2])
    
