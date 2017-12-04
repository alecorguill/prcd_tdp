import unittest,os
from util import *
        
def gemm_fox_random():
    N = 8
    Np = 4
    epsilon = 1e-6
    filename_A = "matrix_A.txt"
    filename_B = "matrix_B.txt"
    filename_C = "matrix_C.txt"
    filename_C_seq = " matrix_C_seq.txt"
    os.system("python generate_matrix.py" + " " + filename_A + " " + str(N))
    os.system("python generate_matrix.py" + " " + filename_B + " " + str(N))
    os.system("mpirun --mca pml ob1 -n " + str(Np) + " ../gemm_fox" + " " + filename_A + " " + filename_B + " " + filename_C)
    A = parse_matrix(filename_A)
    B = parse_matrix(filename_B)
    C_seq = A*B
    C = parse_matrix(filename_C)
    return equal(C,C_seq,N,epsilon)


def gemm_fox():
    N = 8
    Np = 4
    epsilon = 1e-6
    filename_A = "matrix_A.txt"
    filename_B = "matrix_B.txt"
    filename_C = "matrix_C.txt"
    filename_C_seq = " matrix_C_seq.txt"
    matrix = np.matrix([[1]*N]*N)
    save_matrix(matrix,"matrix_A.txt")
    save_matrix(matrix,"matrix_B.txt")
    os.system("mpirun --mca pml ob1 -n " + str(Np) + " ../gemm_fox" + " " + filename_A + " " + filename_B + " " + filename_C)
    C_seq = parse_matrix(filename_A)*parse_matrix(filename_B)
    C = parse_matrix(filename_C)
    return equal(C,C_seq,N,epsilon)

#####
class TestTDP3(unittest.TestCase):
    def test_gemm_fox_random(self):
        self.assertEqual(gemm_fox_random(),True)
   
    def test_gemm_fox(self):
        self.assertEqual(gemm_fox(),True)
    

if __name__ == "__main__":
    os.system("touch matrix_C.txt")
    unittest.main()

