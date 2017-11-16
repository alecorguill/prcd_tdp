#!/usr/bin/env python
#-*- coding: utf-8 -*-
## Ce fichier doit être appelé depuis le répertoire test

import os, sys
import subprocess, time
#import matplotlib.pyplot as plt
#import pandas as pd
import csv

np_default = 5
nb_part_default = 2*50
nb_proc_step = 2
size = 100

def save_csv(filename,x,y,columnx,columny):

    with open(filename, "wb") as csv_file:
        writer = csv.writer(csv_file, delimiter=',')
        writer.writerow([columnx,columny])
        for i in range(len(x)):
            writer.writerow([str(x[i]),str(y[i])])

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
        save_csv('perf_seq.csv',N,time_seq,'size','perf_seq')
    return time_seq

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
        save_csv('perf_par.csv',N,time_par,'size','perf_par')
    return time_par

def speed_par_f(input, output, save_to_csv=False):
    speed_par = []
    os.system("make -C ../ clean")
    os.system("make -C ../ sequentiel")


    ##Speed up
    P = filter(lambda x: nb_part_default%x==0, range(1,nb_part_default))
    os.system("python generate_particules.py {} {}".format(input,nb_part_default))
    for p in P:
        ##
        start = time.time()
        os.system("mpirun -mca pml ob1 -np {} parallel {} {}".format(p,input,output))
        end = time.time()
        speed_par.append(end-start)
    if save_to_csv:
        save_csv('speed_par.csv',P,speed_par,'size','speed_par')
  
    return speed_par

def speed_up_f(speed_par, input, output, save_to_csv=False):
    os.system("make -C ../ clean")
    os.system("make -C ../ parallel")
    os.system("python generate_particules.py {} {}".format('particules_tmp.txt',nb_part_default))

    # Tps sequentiel
    start = time.time()
    os.system("../sequentiel {} {}".format(input,output))
    end = time.time()
    t = end-start
    
    speed_up = [t/s for s in speed_par]
    P = range(2,nb_part_default,2)
    if save_to_csv:
        save_csv('speed_up.csv',P,speed_up,'size','speed_up')
    return speed_up


def perf(input, output):
    time_seq, time_par, speed_seq, speed_par, speed_up = [],[],[],[],[] 
    time_seq = perf_seq(input, output,True)
    time_par = perf_par(input, output,True)
    speed_par = speed_par_f(input, output,True)
    speed_up = speed_up_f(speed_par,input, output, True)
    return time_seq,time_par,speed_par,speed_up


def plot(x,y,name,xlabel=None,ylabel=None,label=None):
    # # Plotting
    fig, ax = plt.subplots()
    ax.set(xlabel=xlabel,ylabel=ylabel,title=name)
    ax.plot(x,y,label)
    
# the csv files need to contain only 2 columns
def plot_csv(filename,name,xlabel=None, ylabel=None, save=False):
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


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python analyse_perf.py input output")
        exit(-1)
        
    time_seq,time_par,speed_par,speed_up = perf(sys.argv[1],sys.argv[2])
    
