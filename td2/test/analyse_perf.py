import os, sys
import matplotlib.pyplot as plt
import subprocess, time
import matplotlib.pyplot as plt

np_default = 5
nb_part_default = 200
nb_proc = range(10)
size = range(100)


def perf(input, output):
    time_seq = []
    time_par = []
    speed_seq = []
    speed_par = []
    
    # os.system("make -C ../ clean")
    # os.system("make -C ../ sequentiel")
    
    # # Perf sequentiel
    # for n in range(1,nb_part_default):
    #     ##
    #     os.system("python generate_particules.py {} {}".format(input,n))
    #     start = time.time()
    #     os.system("../sequentiel {} {}".format(input,output))
    #     end = time.time()
    #     time_seq.append(end-start)

    os.system("make -C ../ clean")
    os.system("make -C ../ parallel")
   
    # Perf parallel
    for n in range(np_default,nb_part_default,np_default):
        ##
        os.system("python generate_particules.py {} {}".format(input,n))
        start = time.time()
        os.system("mpirun -mca pml ob1 -np {} ./parallel {} {}".format(np_default,input,output))
        end = time.time()
        time_par.append(end-start)
    
    #Speed up    

    # os.system("python generate_particules.py filename {} {}".format(input,nb_part_default))
    # for p in range(nb_proc):
    #     ##
    #     start = time.time()
    #     os.system("../sequentiel {} {}".format(input,ouput))
    #     end = time.time()
    #     speed_seq.append(end-start)
    #     start = time.time()
    #     os.system("mpirun -mca pml ob1 -np {} parallel {} {}".format(p,input,ouput))
    #     end = time.time()
    #     speed_par.append(end-start)
    
    return time_seq,time_par,speed_seq,speed_par

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python analyse_perf.py input output")
        exit(-1)
    
    result = perf(sys.argv[1],sys.argv[2])
    # Plotting
    fig, ax = plt.subplots()
    ax.plot(range(np_default,nb_part_default,np_default), result[1])
    plt.show()
