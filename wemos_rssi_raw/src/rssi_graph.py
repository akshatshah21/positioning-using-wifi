    from matplotlib import pyplot as plt 
    # f = open("LogFile.txt","r")
    x = []
    y = []
    count = 0
    for line in open("LogFile.txt","r"):
        temp = [float(s) for s in line.split()]
        y.append(temp)
        count += 1
        x.append(count)
    plt.plot(x,y)

    plt.show()