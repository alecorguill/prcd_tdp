import random
import sys

V_RANGE = 2.5
P_RANGE = 10

def generate_boxes(filename, n1, n2):
    fd = open(filename, "w")
    n = n1+n2
    small_size = 0.1*P_RANGE
    fd.write(str(n1+n2)+'\n')
    fd.write(str(n1)+'\n')
    for i in range(n1):        
        masse = random.randrange(1,5000)
        px = random.uniform(0,small_size)
        py = random.uniform(0,P_RANGE)
        vx = random.uniform(-V_RANGE,V_RANGE)
        vy = random.uniform(-V_RANGE,V_RANGE)
        fd.write(("%s %s %s %s %s\n")% (masse,px,py,vx,vy))
    fd.write(str(n2)+'\n')
    for i in range(n2):        
        masse = random.randrange(1,5000)
        px = random.uniform(P_RANGE-small_size,P_RANGE)
        py = random.uniform(0,small_size)
        vx = random.uniform(-V_RANGE,V_RANGE)
        vy = random.uniform(-V_RANGE,V_RANGE)
        fd.write(("%s %s %s %s %s\n")% (masse,px,py,vx,vy))

    fd.close()

if __name__ == "__main__":
    args = sys.argv
    if len(args) < 4:
        print "USAGE : python generate_particules.py filename n1 n2"
    generate_boxes(args[1],int(args[2]), int(args[3]))                      
