import time

x1 =0.0
y1 =0.0
x2 =1.0
y2 =0.0
x3 =1.0
y3 =4.0

f1 = open("Distance1.txt",'r')
f2 = open("Distance2.txt",'r')
f3 = open("Distance3.txt",'r')
p = open('Position.txt','w')

# while(f1.tell != f1.read()):
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
    # print(r1,r2,r3)

    xNr1 = y3*(r2*r2 - x2*x2 - y2*y2 - r1*r1)
    xNr2 = y2*(r3*r3 - x3*x3 - y3*y3 -r1*r1)
    xDr = 2*(y2*x3 - y3*x2)
    x = (xNr1 - xNr2) / xDr


    yNr1 = x3*(r2*r2 - x2*x2 - y2*y2 - r1*r1)
    yNr2 = x2*(r3*r3 - x3*x3 - y3*y3 - r1*r1)
    yDr = 2*(y3*x2 - y2*x3)

    y = (yNr1 - yNr2) / yDr

    print('Tag: (' + str(x) + ',' + str(y) + ')')
    p.write(str(x))
    p.write(' ')
    p.write(str(y))
    p.write('\n')
    time.sleep(0.2)
    f1.seek(0)
    f2.seek(0)
    f3.seek(0)


    # r1.append(float(t1))
    # r2 =
    # r3 =

    # print(r1)#,r2,r3)
