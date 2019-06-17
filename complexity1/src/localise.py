import time

ssid_list_f = open('ssid_list.txt','r')
ssid_list = f.read().split('\n')
pos_file_list = [open("pos_ssid_{}.txt".format(x),'w') for x in range(10)]

while 1:
    for ssid in range(0,len(ssid_list)):
        file_a1 = open('a1/ssid_{}.txt'.format(ssid.index()),'r')
        file_a2 = open('a2/ssid_{}.txt'.format(ssid.index()),'r')
        file_a3 = open('a3/ssid_{}.txt'.format(ssid.index()),'r')

        list_a1 = file_a1.read().split('\n')
        list_a2 = file_a2.read().split('\n')
        list_a3 = file_a3.read().split('\n')

        d1 = float(list_a1[-2])
        d2 = float(list_s2[-2])
        d3 = float(list_a3[-2])

        xNr1 = y3*(r2*r2 - x2*x2 - y2*y2 - r1*r1)
        xNr2 = y2*(r3*r3 - x3*x3 - y3*y3 -r1*r1)
        xDr = 2*(y2*x3 - y3*x2)
        x = (xNr1 - xNr2) / xDr


        yNr1 = x3*(r2*r2 - x2*x2 - y2*y2 - r1*r1)
        yNr2 = x2*(r3*r3 - x3*x3 - y3*y3 - r1*r1)
        yDr = 2*(y3*x2 - y2*x3)

        y = (yNr1 - yNr2) / yDr

        p = pos_file_list[ssid.index()]

        print('{}:\t( '.format(ssid) + str(x) + ' , ' + str(y) + ' )')
        p.write(str(x))
        p.write(' ')
        p.write(str(y))
        p.write('\n')

        file_a1.close()
        file_a2.close()
        file_a3.close()

        time.sleep(0.2)
