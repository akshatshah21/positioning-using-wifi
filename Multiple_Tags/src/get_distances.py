#!/usr/bin/python3

import paho.mqtt.client as mqtt
import time
import os


'''
file_list_a1 = [open("a1/ssid_{}.txt".format(x),'w') for x in range(10)]
file_list_a2 = [open("a2/ssid_{}.txt".format(x),'w') for x in range(10)]
file_list_a3 = [open("a3/ssid_{}.txt".format(x),'w') for x in range(10)]
'''

file_list_a1 = []
file_list_a2 = []
file_list_a3 = []
ssid_list = []
file_dict_a1 = {} #Empty dictionary for ssid-file object links
file_dict_a2 = {} #Empty dictionary for ssid-file object links
file_dict_a3 = {} #Empty dictionary for ssid-file object links
ssid_count = 0

ssid_list_f = open('ssid_list.txt','w')

def on_message(client, userdata, message):
    # print("Message received")

    global ssid_list, ssid_count, file_dict_a1, file_dict_a2, file_dict_a3, ssid_list_f, file_list_a1,file_list_a2, file_list_a3
    if str(message.topic) == 'ssid':    #Messages will have ssid
        print('ssid')

        ssid_received = str(message.payload.decode())
        if (ssid_received not in ssid_list) and (ssid_received!="JioPrivateNet") and (ssid_received!="JioNet@VJTI_Matunga") and (ssid_received!="VJTI-AP3") and  (ssid_received!="COE_AVITRA_5G") :
            ssid_count += 1
            ssid_list.append(ssid_received)

            ssid_list_f.write(ssid_received + '\n')
            ssid_list_f.flush()
            os.fsync(ssid_list_f.fileno())
            file_list_a1.append(open("a1/{}.txt".format(ssid_received),'w'))
            print("Added SSID to a1")
            file_list_a2.append(open("a2/{}.txt".format(ssid_received),'w'))
            print("Added SSID to a2")
            file_list_a3.append(open("a3/{}.txt".format(ssid_received),'w'))
            print("Added SSID to a3")
            # file_dict_a1[ssid_received] = file_list_a1[ssid_count - 1]
            print(file_list_a1)
            file_dict_a1[ssid_received] = file_list_a1[ssid_count - 1]
            print("Dict a1:")
            print(file_dict_a1)
            file_dict_a2[ssid_received] = file_list_a2[ssid_count - 1]
            file_dict_a3[ssid_received] = file_list_a3[ssid_count - 1]
    elif str(message.topic) == 'a1':      #Message in the form of ssid:distance
        print("a1 sent message")
        msg = message.payload.decode()
        print("Message from a1 decoded")
        print(str(msg))
        separated_msg = msg.split(':')
        print("Message splitted")
        ssid = str(separated_msg[0])
        for temp in ssid_list:
            print("Entered inner for loop of a1")
            if ssid == temp:
                print("SSID found inside a1")
                f = file_dict_a1[ssid]
                print("********************Writing to",ssid,"file of a1")
                f.write(separated_msg[1] + '\n')
                f.flush()
                os.fsync(f.fileno())

    elif str(message.topic) == 'a2':
        print('a2')
        msg = message.payload.decode()
        print("Message from a2 decoded")
        print(str(msg))
        separated_msg = msg.split(':')
        print("Message splitted")
        ssid = str(separated_msg[0])
        for temp in ssid_list:
            print("Entered inner for loop of a2")
            if ssid == temp:
                print("SSID found inside a2")
                f = file_dict_a2[ssid]
                print("Writing to",ssid,"file of a2")
                f.write(separated_msg[1] + '\n')
                f.flush()
                os.fsync(f.fileno())
    elif str(message.topic) == 'a3':
        print('a3')
        msg = message.payload.decode()
        print("Message from a1 decoded")
        print(str(msg))
        separated_msg = msg.split(':')
        print("Message splitted")
        ssid = str(separated_msg[0])
        for temp in ssid_list:
            print("Entered inner for loop of a3")
            if ssid == temp:
                print("SSID found inside a3")
                f = file_dict_a3[ssid]
                print("Writing to",ssid,"file of a3")
                f.write(separated_msg[1] + '\n')
                f.flush()
                os.fsync(f.fileno())

# def on_connect(client, userdata,flags,rc):
    # print("Connected")
    # mqttClient.on_message = on_message



broker = 'postman.cloudmqtt.com'
mqttUsername = 'ocbshoyv'
mqttPwd = 'u7RF9Xts1g1r'
mqttPort = 16557
mqttClient = mqtt.Client('Laptop',True)

# global mqttClient
mqttClient.on_message = on_message
# mqttClient.on_connect = on_connect
print("Connecting to",broker)
mqttClient.username_pw_set(username = mqttUsername, password = mqttPwd)
mqttClient.connect(broker,mqttPort)
# print("Connected")
print("Username pwd set")
mqttClient.subscribe([('a1',0),('a2',0),('a3',0),('ssid',0)])
print("Subscribed")

print("on_message function connected")


# setup_mqtt()
# mqttClient.loop_start()
# print("in loop")
# time.sleep(15)
# mqttClient.loop_stop()
mqttClient.loop_forever()
print("After loop")
