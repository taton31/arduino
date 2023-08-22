import matplotlib.pyplot as plt
from matplotlib.widgets import CheckButtons

with open("Log.txt", 'r') as F:
    read_data = F.readlines()

names = read_data[0].strip().split(';')
names = list(filter(None, map(lambda x: x[:x.find('=') if x.find('=') != -1 else 0], names)))

# names = names[1:]############################
data = list(map(lambda x: x.strip().split(';'), read_data))
for i in range(len(data)):
    data[i] = list(map(lambda x: x[x.find('=')+1:], data[i]))

data_X = list(map(lambda x: x[0], data))
data_Y = list(map(lambda x: x[1:], data))

fig, ax = plt.subplots()

plots=[]
for i in range(len(data_Y[0])):
    plots.append(ax.plot(data_X,list(map(lambda x: float(x[i]), data_Y)), label=f'{names[i]}')[0])
# print(plots)
fig.subplots_adjust(left=0.06)
fig.legend(names, loc='upper left')

rax = fig.add_axes([0, 0.1, 0.025, 0.5])
# rax = fig.add_axes([0, 1-0.15 * len(names), 0.025, 0.05  * len(names)])
labels = [str(line.get_label()) for line in plots]
visibility = [line.get_visible() for line in plots]
check = CheckButtons(rax, labels, visibility)


def func(label):
    index = labels.index(label)
    plots[index].set_visible(not plots[index].get_visible())

    plt.draw()

check.on_clicked(func)
plt.rcParams["figure.autolayout"] = True

plt.get_current_fig_manager().window.state('zoomed')
plt.show()


