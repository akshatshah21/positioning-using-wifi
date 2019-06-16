# import paho.mqtt.client as mqtt
import time
import os

count = 1
print('One:',count)
def inc():
    global count
    # count += 1
    print('Two:',count)
inc()
print('Three:',count)
'''
broker = "postman.cloudmqtt.com"

def setup_mqtt():
    mqtt mqttClient('Laptop')
    mqttClient.
'''
