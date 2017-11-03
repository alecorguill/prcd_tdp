import os, sys
import csv
import matplotlib.pyplot as plt

def plot_csv(filename, save=False):
    # Extracting
    with open(filename) as csvfile:
        rows = list(csv.reader(csvfile))
    features = rows.pop(0)
    features = features[:2]
    sizes = map(lambda x:int(x[0]), rows)
    rows = map(lambda x:x[1:], rows)

    # Plotting
    name = filename.split('.')[0]
    fig, ax = plt.subplots()
       
    for ind_feature in range(0,len(features)-1):

        feature = features[ind_feature+1]
        flops = map(lambda x:float(x[ind_feature]), rows)
        
        ax.plot(sizes, flops,label=feature)
        ax.set(xlabel='Size of the matrix (N)', ylabel='MFlop per second (Mflop/s)',
               title= name)
        axes = plt.gca()
        axes.set_xlim([0,sizes[-1]])
    if save:
        fig.savefig(name + '_' + ''.join(feature.split()));

if __name__ == '__main__':
    args = sys.argv
    for i in range(1, len(args)):
        plot_csv(args[i])
    #plt.legend()
    plt.show()
