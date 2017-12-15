import sys
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

# Get filename
try:
    filename = sys.argv[1]
except:
    filename = 'output.txt'

array = []
values = {}


def to_float(x):
    try:
        return float(x)
    except ValueError:
        return None
    
# Parsing file into an array of dictionnaries
with open(filename) as fp:
    for line in fp:
        l = line.split(',')
        l = map(lambda x:to_float(x),l)
        l = filter(lambda x: x != None,l)
        # time is the first line of each step
        if len(l) == 0:
            continue
        elif len(l) < 2:
            if values!={}:
                array.append(values)
            ##
            values = {'t':0,'pos':[]}
            values['t'] = l[0]
        else:
            values['pos'].append(l)

# Last measure
array.append(values)
#print array
# Plotting 2D animation
def plot():
    fig = plt.figure()
    ax = fig.add_subplot(111, autoscale_on=False, xlim=(-50, 50), ylim=(-50, 50))
    ax.grid()
    
    line, = ax.plot([], [], 'o', lw=2)
    time_template = 'time = %r'
    time_text = ax.text(0.05, 0.9, '', transform=ax.transAxes)


    def init():
        line.set_data([], [])
        time_text.set_text('')
        return line, time_text


    def animate(i):
        thisx = map(lambda x:x[0], array[i]['pos'])
        thisy = map(lambda x:x[1], array[i]['pos'])
        line.set_data(thisx, thisy)
        time_text.set_text(time_template % array[i]['t'])
        return line, time_text

    ani = animation.FuncAnimation(fig, animate, np.arange(0, len(array)),
                                  interval=500, blit=False, init_func=init)
    plt.show()
plot()
if __name__ == '__main__':
    pass
