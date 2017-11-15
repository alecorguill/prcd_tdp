import random
import sys

V_RANGE = 2.5
P_RANGE = 10

def generate_particules(filename, n):
    fd = open(filename, "w")
    fd.write(str(n)+'\n')
    for i in range(n):
        masse = random.randrange(1,5000)
        px = random.uniform(0,P_RANGE)
        py = random.uniform(0,P_RANGE)
        vx = random.uniform(-V_RANGE,V_RANGE)
        vy = random.uniform(-V_RANGE,V_RANGE)
        fd.write(("%s %s %s %s %s\n")% (masse,px,py,vx,vy))
    fd.close()

if __name__ == "__main__":
    args = sys.argv
    if len(args) < 3:
        print "USAGE : python generate_particules.py filename nb_particule"
    generate_particules(args[1],int(args[2]))                      
