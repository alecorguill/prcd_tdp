import random
import sys

def encoder(input,output):
    nb_boxes = 0
    with open(input) as fi:
        with open(output,'w') as fo:
            fo.write(fi.readline())
            for line in fi:
                if (len(line.split(' ')) != 1):
                    fo.write(line)
                
if __name__ == "__main__":
        args = sys.argv
        if len(args) != 3:
            print "Usage : python encoder.py input output"
        else:
            input = args[1]
            output = args[2]
            encoder(input,output)
