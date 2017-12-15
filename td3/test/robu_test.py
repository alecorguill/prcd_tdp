from util import *
import math,os

def diff_seq_parN():
    N = 8
    Np = 4
    N_lim = 50
    measures = []
    filename_A = "matrix_A.txt"
    filename_B = "matrix_B.txt"
    filename_C = "matrix_C.txt"
    filename_C_seq = " matrix_C_seq.txt"
    size = math.sqrt(Np)
    l = []
    val = 8
    while val < N_lim:
        val = int(val*1.3)
        l.append(val - val%4)

    for N in l:
        os.system("python generate_matrix.py" + " " + filename_A + " " + str(N))
        os.system("python generate_matrix.py" + " " + filename_B + " " + str(N))
        os.system("mpirun --mca pml ob1 -n " + str(Np) + " ../gemm_fox" + " " + filename_A + " " + filename_B + " " + filename_C)
        A = parse_matrix(filename_A)
        B = parse_matrix(filename_B)
        C_seq = A*B
        C = parse_matrix(filename_C)
        measures.append([N,np.linalg.norm(C_seq-C)/np.linalg.norm(C_seq)])
    write_csv(measures,["N", "erreur"],"erreurN.csv")

diff_seq_parN();

# def diff_seq_parNp():
#     N = 8
#     Np = 4
#     Np_lim =  1000
#     filename_A = "matrix_A.txt"
#     filename_B = "matrix_B.txt"
#     filename_C = "matrix_C.txt"
 
#     measures = []
#     l = [x**2 for x in range(1,Np_lim) if N%x == 0]
#     print("python generate_matrix.py" + " " + filename_A + " " + str(N))
#     print("python generate_matrix.py" + " " + filename_B + " " + str(N))
    
#     os.system("python generate_matrix.py" + " " + filename_A + " " + str(N))
#     os.system("python generate_matrix.py" + " " + filename_B + " " + str(N))
#     for Np in l:
#         print("mpirun --mca pml ob1 -n " + str(Np) + " ../gemm_fox" + " " + filename_A + " " + filename_B + " " + filename_C)
#         ##	
# 	result = os.popen("mpirun -n " + str(Np) + " ../gemm_fox" + " " + filename_A + " " + filename_B + " " + filename_C).read();
# 	result = result.split('\n')[-2]
# 	time = float(result)
#         A = parse_matrix(filename_A)
#         B = parse_matrix(filename_B)
#         C_seq = A*B
#         C = parse_matrix(filename_C)
#         measures.append([Np,np.linalg.norm(C_seq-C)])
#     write_csv(measures,["N", "erreur"],"erreurNp.csv")

