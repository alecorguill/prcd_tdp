import matplotlib as plt
while open('flop.csv') as fp:
    for line in fp:
        tmp = line.split(',')
        sizes += [tmp[0]]
        flops += [tmp[1]]
