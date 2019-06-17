import paho.mqtt.client as mqtt
import time
import os

broker = 'postman.cloudmqtt.com'
mqttUsername = 'ocbshoyv'
mqttPwd = 'u7RF9Xts1g1r'
mqttPort = 16557
mqttClient = mqtt.Client('Laptop',True)

file_list_a1 = [open("a1/file_{}".format(x),'w') for x in range(10)]
file_list_a2 = [open("a2/file_{}".format(x),'w') for x in range(10)]
file_list_a3 = [open("a3/file_{}".format(x),'w') for x in range(10)]

ssid_list = []
file_dict = {} #Empty dictionary for ssid-file object links
ssid_count = 0
def on_message(client, userdata, message):
    global ssid_list,ssid_count,file_dict
    if str(message.topic) == 'ssid':    #Messages will have ssid
        ssid_count += 1
        ssid_received = str(message.payload.decode())
        if ssid_received not in ssid_list:
            ssid_list.append(ssid_received)
            file_dict[ssid_received] = file_list[ssid_count - 1]
    if str(message.topic) == 'a1':      #Message in the form of ssid:distance
        msg = message.payload.decode()
        print(msg)
        separated_msg = msg.split(':')
        ssid = str(separated_msg[0])
        for temp in ssid_list:
            if ssid == temp:
                f = file_list_a1[temp.index()]
                f.write(separated_msg[1] + '\n')
                #Use dictionary here. temp must be key. Return value must be file object
    if str(message.topic) == 'a2':
        msg = message.payload.decode()
        print(msg)
        separated_msg = msg.split(':')
        ssid = str(separated_msg[0])
        for temp in ssid_list:
            if ssid == temp:
                f = file_list_a2[temp.index()]
                f.write(separated_msg[1] + '\n')




def setup_mqtt():
    global mqttClient
    mqttClient.connect(broker,mqttPort)
    mqttClient.username_pw_set(username = mqttUsername, password = mqttPwd)
    mqttClient.subscribe([(a1,0),(a2,0),(a3,0)])
    mqttClient.on_message = on_message(mqttClient)
