import matplotlib.pyplot as plt
sizes, flops = [], []
with open('flop.csv') as fp:
    for line in fp:
        tmp = line.split(',')
        if tmp[0] == 'size':
            continue
        sizes += [int(tmp[0])]
        flops += [float(tmp[1])]

# Plotting 
fig, ax = plt.subplots()
ax.plot(sizes, flops)
ax.set(xlabel='Size of the matrix (N)', ylabel='Flop per second (flops)',
       title='Relationship between Flops and size of a given matrix')
fig.savefig("flops_and_sizes")
plt.show()
