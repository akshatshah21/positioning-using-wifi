import paho.mqtt.client as mqtt
import time
import os

broker = 'postman.cloudmqtt.com'
mqttUsername = 'ocbshoyv'
mqttPwd = 'u7RF9Xts1g1r'
mqttPort = 16557
mqttClient = mqtt.Client('Laptop',True)

file_list = [open("file_{}".format(x),'w') for x in range(10)]

ssid_list = []
file_dict = {} #Empty dictionary for ssid-file object links
ssid_count = 0
def on_message(client, userdata, message):
    global ssid_list,ssid_count,file_dict
    if str(message.topic) == 'ssid':
        ssid_count += 1
        ssid_list.append(str(message.payload.decode()))
        file_dict[ssid_count - 1] = file_list[ssid_count - 1]
    if str(message.topic) == 'a1':
        msg = message.payload.decode()
        print(msg)
        separated_msg = msg.split(':')
        ssid = str(separated_msg[0])
        for temp in ssid_list:
            if ssid == temp:
                f = file_list[temp.index()]
                f.write(separated_msg[1] + '\n')
                #Use dictionary here. temp must be key. Return value must be file object





def setup_mqtt():
    global mqttClient
    mqttClient.connect(broker,mqttPort)
    mqttClient.username_pw_set(username = mqttUsername, password = mqttPwd)
    mqttClient.subscribe([(a1,0),(a2,0),(a3,0)])
    mqttClient.on_message = on_message(mqttClient)
