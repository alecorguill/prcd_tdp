try:
    import numpy as np
except ImportError:
    print "Numpy is not available on your computer"
else:
    def parse_matrix(filename):
        matrix = []
        with open(filename.strip()) as fp:
            for line in fp:
                coeffs = line.split('\n')[0].split(' ')
                del coeffs[-1]
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
finally:    
    def write_csv(array, headers, csvfilename):
        with open(csvfilename, 'w') as fp:
            fp.write("{} \n".format(",".join(headers)))
            for row in array:
                row = map(lambda x:str(x),row)
                fp.write("{} \n".format(",".join(row)))
