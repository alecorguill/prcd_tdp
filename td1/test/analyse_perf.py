import csv
import matplotlib.pyplot as plt

def plot_csv(filename, save=False):
    # Extracting
    with open(filename) as csvfile:
        rows = list(csv.reader(csvfile))
    features = rows.pop(0) 
    sizes = map(lambda x:int(x[0]), rows)
    rows = map(lambda x:x[1:], rows)

    # Plotting
    name = filename.split('.')[0]
    for ind_feature in range(0,len(features)-1):

        feature = features[ind_feature+1]
        fig, ax = plt.subplots()
        flops = map(lambda x:float(x[ind_feature]), rows)
        
        ax.plot(sizes, flops)
        ax.set(xlabel='Size of the matrix (N)', ylabel='Flop per second (flops)',
               title= name + feature)
        if save:
            fig.savefig(name + '_' + ''.join(feature.split()));

plot_csv('flop_matrice.csv', True)
