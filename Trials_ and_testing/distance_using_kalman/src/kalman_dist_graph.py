from matplotlib import pyplot as plt 
# f = open("LogFile.txt","r")
x = []
normal_dist = []
#pred_dist = []
kalman_dist = []
count = 0
for line in open("LogFile.txt","r"):
    temp = [float(s) for s in line.split()]
    kalman_dist.append(temp[0])
    normal_dist.append(temp[1])
    #kalman_dist.append(temp[3])
    count += 1
    x.append(count)
plt.plot(x,kalman_dist)
plt.plot(x,normal_dist)
#plt.plot(x,kalman_dist)

plt.show()
