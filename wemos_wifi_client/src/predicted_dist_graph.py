from matplotlib import pyplot as plt 
# f = open("LogFile.txt","r")
x = []
actual_dist = []
pred_dist = []
kalman_dist = []
count = 0
for line in open("LogFile.txt","r"):
    temp = [float(s) for s in line.split()]
    pred_dist.append(temp[1])
    actual_dist.append(temp[2])
    kalman_dist.append(temp[3])
    count += 1
    x.append(count)
plt.plot(x,actual_dist)
plt.plot(x,pred_dist)
plt.plot(x,kalman_dist)

plt.show()
