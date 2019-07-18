import matplotlib.pyplot as plt
import matplotlib.patches as patches
import matplotlib.animation as animation
import time
f1 = open("Distance1.txt",'r')
f2 = open("Distance2.txt",'r')
f3 = open("Distance3.txt",'r')
fig = plt.figure()
ax1 = fig.add_subplot(1,1,1)

x1 =0.0
y1 =0.0
x2 =3.0
y2 =0.0
x3 =0.0
y3 =3.0

def plotCircle(r1,r2,r3):
    print("Entered plotCircle()")
    c1 = patches.Circle((x1,y1),r1,facecolor='red',alpha=0.3,edgecolor='yellow',linestyle='dotted',linewidth='2.2')
    c2 = patches.Circle((x2,y2),r2,facecolor='green',alpha=0.3,edgecolor='yellow',linestyle='dotted',linewidth='2.2')
    c3 = patches.Circle((x3,y3),r3,facecolor='blue',alpha=0.3,edgecolor='yellow',linestyle='dotted',linewidth='2.2')
    plt.gca().add_patch(c1)
    plt.gca().add_patch(c2)
    plt.gca().add_patch(c3)
    plt.axis('scaled')
    plt.grid()
    plt.title('Circles')
    # plt.show()
    time.sleep(3)
    print("Exiting plotCircle()")

def animate(i):
    # ax1.clear()
    print("Entered animate()")
    while 1:
        t1 = f1.read()
        t2 = f2.read()
        t3 = f3.read()
        l1 = t1.split('\n')
        l2 = t2.split('\n')
        l3 = t3.split('\n')

        r1 = float(l1[-2])
        r2 = float(l2[-2])
        r3 = float(l3[-2])
        plotCircle(r1,r2,r3)
        print("Circles plotted")

ani = animation.FuncAnimation(fig, animate, interval = 1000)
plt.show()
