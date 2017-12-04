import random,sys
import numpy as np

MIN_VAL = 2.5
MAX_VAL = 10

def generate_row(N):
    row = np.random.uniform(MIN_VAL,MAX_VAL,N)
    row = map(lambda x:str(x), row)
    return ' '.join(row)

def generate_matrix(filename, N):
    with open(filename, "w") as fd:
        fd.write(str(N)+'\n')
        for i in range(N): 
            row = generate_row(N)
            fd.write(row+' \n')
     
            
if __name__ == "__main__":
    args = sys.argv
    if len(args) < 3:
        print "Usage : python generate_matrix.py filename N"
    generate_matrix(args[1],int(args[2]))                      
