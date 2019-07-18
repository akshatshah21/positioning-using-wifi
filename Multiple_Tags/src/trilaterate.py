import time

x1 =0.0
y1 =0.0
x2 =0.0
y2 =3.0
x3 =3.0
y3 =1.5

ssid_list_f = open('ssid_list.txt','r')
ssid_list = ssid_list_f.read().split('\n')
pos_file_list = [open("pos/pos_ssid_{}.txt".format(x),'w') for x in range(10)]
file_a1 = None
file_a2 = None
file_a3 = None

while 1:
    for ssid in ssid_list:
        if ssid != '':
            file_a1 = open('a1/{}.txt'.format(ssid),'r')
            file_a2 = open('a2/{}.txt'.format(ssid),'r')
            file_a3 = open('a3/{}.txt'.format(ssid),'r')
            file_a1.seek(0)
            file_a2.seek(0)
            file_a3.seek(0)
            list_a1 = file_a1.read().split('\n')
            list_a2 = file_a2.read().split('\n')
            list_a3 = file_a3.read().split('\n')

            r1 = float(list_a1[-2])
            r2 = float(list_a2[-2])
            r3 = float(list_a3[-2])


            xNr1 = y3*(r2*r2 - x2*x2 - y2*y2 - r1*r1)
            xNr2 = y2*(r3*r3 - x3*x3 - y3*y3 -r1*r1)
            xDr = 2*(y2*x3 - y3*x2)
            x = (xNr1 - xNr2) / xDr


            yNr1 = x3*(r2*r2 - x2*x2 - y2*y2 - r1*r1)
            yNr2 = x2*(r3*r3 - x3*x3 - y3*y3 - r1*r1)
            yDr = 2*(y3*x2 - y2*x3)

            y = (yNr1 - yNr2) / yDr

            p = pos_file_list[ssid_list.index(ssid)]

            print('{}:\t( '.format(ssid) + str(x) + ' , ' + str(y) + ' )')
            p.write(str(x))
            p.write(' ')
            p.write(str(y))
            p.write('\n')

            file_a1.close()
            file_a2.close()
            file_a3.close()

    time.sleep(2)
