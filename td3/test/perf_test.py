import csv
import os

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
    for n in range(N_lim):
        N = n**2
        os.system("python generate_matrix.py" + filename_A + " " + str(N))
        os.system("python generate_matrix.py" + filename_B + " " + str(N))
        time = float(os.popen("mpirun -n " + str(Np) + " ../gemm_fox" + filename_A + filename_B + filename_C).read())
        measures.append([n,time])
    return measures


def gemm_fox_Np(N, Np_lim):
    filename_A = " matrix_A.txt"
    filename_B = " matrix_B.txt"
    filename_C = " matrix_C.txt"
    measures = []
    for Np in range(1,Np_lim,2):
        os.system("python generate_matrix.py" + filename_A + " " + str(N))
        os.system("python generate_matrix.py" + filename_B + " " + str(N))
        time = float(os.popen("mpirun -n " + str(Np) + " ../gemm_fox" + filename_A + filename_B + filename_C).read())
        measures.append([Np,time])
    return measures

if __name__ == '__main__':
    if len(sys.argv) != 2:
        
    
