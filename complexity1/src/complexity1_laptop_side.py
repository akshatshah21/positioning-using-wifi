# import paho.mqtt.client as mqtt
import time
import os


#MQTT parameters
broker = 'postman.cloudmqtt.com'
mqttUserName = 'ocbshoyv'
mqttPwd = 'u7RF9Xts1g1r'
mqttPort = 16557
mqttClient = mqtt.client('Laptop',True)


'''
Setting up the MQTT Client
Uses the parameters as above
'''
def setup_mqtt():
    global mqttClient
    mqttClient.username_pw_set(username = mqttUserName, password = mqttPwd)
    mqttClient.on_message = on_message()
    print("Connecting to broker:"+broker)
    mqttClient.connect(broker,mqttPort)
    print("Connected")
    mqttClient.subscribe([(a1,0),(a2,0),(a3,0)])
