#!/usr/bin/python3

import paho.mqtt.client as mqtt
import time
import os



file_list_a1 = [open("a1/file_{}".format(x),'w') for x in range(10)]
file_list_a2 = [open("a2/file_{}".format(x),'w') for x in range(10)]
file_list_a3 = [open("a3/file_{}".format(x),'w') for x in range(10)]

ssid_list = []
file_dict_a1 = {} #Empty dictionary for ssid-file object links
file_dict_a2 = {} #Empty dictionary for ssid-file object links
file_dict_a3 = {} #Empty dictionary for ssid-file object links
ssid_count = 0

def on_message(client, userdata, message):
    print("Message received")
    '''
    global ssid_list,ssid_count,file_dict_a1,file_dict_a2,file_dict_a3
    if str(message.topic) == 'ssid':    #Messages will have ssid
        ssid_count += 1
        ssid_received = str(message.payload.decode())
        if ssid_received not in ssid_list:
            ssid_list.append(ssid_received)
            file_dict_a1[ssid_received] = file_list_a1[ssid_count - 1]
            file_dict_a2[ssid_received] = file_list_a2[ssid_count - 1]
            file_dict_a3[ssid_received] = file_list_a3[ssid_count - 1]
    elif str(message.topic) == 'a1':      #Message in the form of ssid:distance
        print("Hello a1")
        msg = message.payload.decode()
        print(str(msg))
        separated_msg = msg.split(':')
        ssid = str(separated_msg[0])
        for temp in ssid_list:
            if ssid == temp:
                f = file_dict_a1[ssid]
                f.write(separated_msg[1] + '\n')
                f1.flush()
                os.fsync(f1.fileno())
                #Use dictionary here. temp must be key. Return value must be file object
    elif str(message.topic) == 'a2':
        msg = message.payload.decode()
        print(str(msg))
        separated_msg = msg.split(':')
        ssid = str(separated_msg[0])
        for temp in ssid_list:
            if ssid == temp:
                f = file_dict_a2[ssid]
                f.write(separated_msg[1] + '\n')
                f1.flush()
                os.fsync(f1.fileno())
    elif str(message.topic) == 'a3':
        msg = message.payload.decode()
        print(str(msg))
        separated_msg = msg.split(':')
        ssid = str(separated_msg[0])
        for temp in ssid_list:
            if ssid == temp:
                f = file_dict_a3[ssid]
                f.write(separated_msg[1] + '\n')
                f1.flush()
                os.fsync(f1.fileno())
    '''
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
mqttClient.connect(broker,mqttPort)
# print("Connected")
mqttClient.username_pw_set(username = mqttUsername, password = mqttPwd)
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
