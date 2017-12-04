
import csv,os,sys

def write_csv(array, headers, csvfilename):
    with open(csvfilename, 'w') as fp:
        fp.write("{} \n".format(",".join(headers)))
        for row in array:
            row = map(lambda x:str(x),row)
            fp.write("{} \n".format(",".join(row)))


def gemm_fox_N(Np, N_lim):
    filename_A = " matrix_A.txt"
    filename_B = " matrix_B.txt"
    filename_C = " matrix_C.txt"
    measures = []
    N = np
    while N < N_lim:
        print("python generate_matrix.py" + filename_A + " " + str(N))
        print("python generate_matrix.py" + filename_B + " " + str(N))
        os.system("python generate_matrix.py" + filename_A + " " + str(N))
        os.system("python generate_matrix.py" + filename_B + " " + str(N))
        print("mpirun -n " + str(Np) + " ../gemm_fox" + filename_A + filename_B + filename_C)
         
        time = float(os.popen("mpirun --mca pml ob1 -n " + str(Np) + " ../gemm_fox" + filename_A + filename_B + filename_C).read())
        measures.append([n,time])
        N = N*2
    return measures


# N has to be divisile by all squared numbers (2²,3²,4²...,Np_lim²)
def gemm_fox_Np(N, Np_lim):
    filename_A = " matrix_A.txt"
    filename_B = " matrix_B.txt"
    filename_C = " matrix_C.txt"
    measures = []
    l = [x*x for x in range(Np_lim)]
    for Np in l:
        print("python generate_matrix.py" + filename_A + " " + str(N))
        print("python generate_matrix.py" + filename_B + " " + str(N))

        os.system("python generate_matrix.py" + filename_A + " " + str(N))
        os.system("python generate_matrix.py" + filename_B + " " + str(N))

        print("mpirun  -n " + str(Np) + " ../gemm_fox" + filename_A + filename_B + filename_C)
        ##
        time = float(os.popen("mpirun --mca pml ob1  -n " + str(Np) + " ../gemm_fox" + filename_A + filename_B + filename_C).read())
        measures.append([Np,time])
    return measures

if __name__ == '__main__':
    if len(sys.argv) < 3:
        print("Usage: python perf_test.py filename N N_lim")
        exit(-1)
    filename = sys.argv[1]
    N = int(sys.argv[2])
    N_lim = int(sys.argv[2])
    measures = gemm_fox_Np(N,N_lim)
    write_csv(measures,["N", "time"],filename)
        
