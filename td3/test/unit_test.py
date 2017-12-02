import unittest,os
import numpy as np

def parse_matrix(filename):
    matrix = []
    with open(filename) as fp:
        for line in fp:
            coeffs = line.split(' ')
            if len(coeffs) < 2:
                continue
            matrix.append(map(lambda x:float(x),coeffs))
    return np.matrix(matrix)
        
def save_matrix(matrix, filename):
    n = len(matrix)
    matrix = np.asarray(matrix)
    with open(filename, 'w') as fp:
        fp.write("{} \n".format(n))
        for l in matrix:
            row = map(lambda x:str(x),l)
            fp.write("{} \n".format(" ".join(row)))
    
def equal(A, B, n, epsilon):
    return (np.linalg.norm(A-B)/(n*epsilon) < 10)
        
def gemm_fox_random():
    N = 5
    Np = 5
    epsilon = 1e-6
    filename_A = " matrix_A.txt"
    filename_B = " matrix_B.txt"
    filename_C = " matrix_C.txt"
    filename_C_seq = " matrix_C_seq.txt"
    os.system("python generate_matrix.py" + filename_A + " " + str(N))
    os.system("python generate_matrix.py" + filename_B + " " + str(N))
    os.system("mpirun -n " + str(Np) + filename_A + filename_B + filename_C)
    os.system("../gemm " + str(Np) + filename_A + filename_B + filename_C_seq)
    C_seq = parse_matrix(filename_C_seq)
    C = parse_matrix(filename_C)
    return equal(C,C_seq,n,epsilon)


def gemm_fox():
    N = 5
    Np = 5
    epsilon = 1e-6
    filename_A = " matrix_A.txt"
    filename_B = " matrix_B.txt"
    filename_C = " matrix_C.txt"
    filename_C_seq = " matrix_C_seq.txt"
    matrix = np.matrix([[1]*N]*N)
    save_matrix(matrix," matrix_A.txt")
    save_matrix(matrix," matrix_B.txt")
    os.system("mpirun -n " + str(Np) + filename_A + filename_B + filename_C)
    os.system("../gemm " + str(Np) + filename_A + filename_B + filename_C_seq)
    C_seq = parse_matrix(filename_C_seq)
    C = parse_matrix(filename_C)
    return equal(C,C_seq,n,epsilon)

#####
class TestTDP3(unittest.TestCase):
    def test_gemm_fox_random(self):
        self.assertEqual(gemm_fox_random(),True)
    
    def test_gemm_fox(self):
        self.assertEqual(gemm_fox(),True)
    

if __name__ == "__main__":
    pass
    #unittest.main(verbosity=2)

