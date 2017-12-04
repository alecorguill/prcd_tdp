# coding: utf-8
import csv,os,sys, math

def gemm_fox_N(Np, N_lim):
    filename_A = " matrix_A.txt"
    filename_B = " matrix_B.txt"
    filename_C = " matrix_C.txt"
    measures = []
    size = math.sqrt(Np)
    l = []
    val = 8
    while val < N_lim:
        val = int(val*1.2)
        l.append(val - val%4)
        
    for N in l:
        print("python generate_matrix.py" + filename_A + " " + str(N))
        print("python generate_matrix.py" + filename_B + " " + str(N))
        os.system("python generate_matrix.py" + filename_A + " " + str(N))
        os.system("python generate_matrix.py" + filename_B + " " + str(N))
        print("mpirun -n " + str(Np) + " ../gemm_fox" + filename_A + filename_B + filename_C)
         
        result = os.popen("mpirun -n " + str(Np) + " ../gemm_fox" + filename_A + filename_B + filename_C).read()
	result = result.split('\n')[-2]
	time = float(result)
        measures.append([N,time])

    return measures


# N has to be divisile by all squared numbers (2²,3²,4²...,Np_lim²)
def gemm_fox_Np(N, Np_lim):
    filename_A = " matrix_A.txt"
    filename_B = " matrix_B.txt"
    filename_C = " matrix_C.txt"
    measures = []
    l = [x**2 for x in range(1,Np_lim) if N%x == 0]
    print("python generate_matrix.py" + filename_A + " " + str(N))
    print("python generate_matrix.py" + filename_B + " " + str(N))
    
    os.system("python generate_matrix.py" + filename_A + " " + str(N))
    os.system("python generate_matrix.py" + filename_B + " " + str(N))    
    for Np in l:
        print("mpirun  -n " + str(Np) + " ../gemm_fox" + filename_A + filename_B + filename_C)
        ##	
	result = os.popen("mpirun -n " + str(Np) + " ../gemm_fox" + filename_A + filename_B + filename_C).read();
	result = result.split('\n')[-2]
	time = float(result)
        measures.append([Np,time])
    return measures

if __name__ == '__main__':
    if len(sys.argv) < 3:
        print("Usage: python perf_test.py filename N N_lim")
        exit(-1)
    filename = sys.argv[1]
    N = int(sys.argv[2])
    N_lim = int(sys.argv[3])
    measures = gemm_fox_Np(N,N_lim)
    #tmp = measures[0][1]
    
    #mesures = [tmp/x for x in measures]
    print measures
    write_csv(measures,["N", "time"],filename)
        
