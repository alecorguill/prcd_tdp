import random
import sys

V_RANGE = 2.5
P_RANGE = 10
M_RANGE = 10E15
def generate_boxes(filename, nb_boxes, nb_part):
    fd = open(filename, "w")
    n = nb_boxes*nb_boxes*nb_part
    box_size = P_RANGE/nb_boxes
    fd.write(str(n)+'\n')
    for i in range(nb_boxes):
        for j in range(nb_boxes):
            fd.write(str(nb_part)+'\n')            
            for k in range(nb_part):        
                masse = random.randrange(1,M_RANGE)
                px = random.uniform(i*box_size,(i+1)*box_size)
                py = random.uniform(j*box_size,(j+1)*box_size)
                vx = random.uniform(-V_RANGE,V_RANGE)
                vy = random.uniform(-V_RANGE,V_RANGE)
                fd.write(("%s %s %s %s %s\n")% (masse,px,py,vx,vy))

    fd.close()

if __name__ == "__main__":
    args = sys.argv
    if len(args) < 4:
        print "USAGE : python generate_particules.py filename nb_boxes nb_part"
        exit(0)
    generate_boxes(args[1],int(args[2]), int(args[3]))                      
