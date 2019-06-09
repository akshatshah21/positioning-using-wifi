#!python3
#Header files
import paho.mqtt.client as mqtt
import time

# On message function for displaying the message 'on_message'
def on_message(client, userdata, message):
	print("Message:",str(message.payload.decode(utf-8)))

broker = "postman.cloudmqtt.com" #clientID. May be IP address also

# The client name is used by the MQTT broker to identify the client.
mqttClient = mqtt.Client("python1")

# Attaching on_message function
mqttClient.on_message = on_message

# Set username and pwd for CloudMQTT Auth
mqttClient.username_pw_set(username = "ocbshoyv",password = "u7RF9Xts1g1r")

#Connect to broker
print("Connecting to broker:"+broker)
mqttClient.connect(broker,16557) # Param 1: broker, Param 2:Port Number
print("Connected")

#Publish a message 'message' to 'example' topic
mqttClient.publish("example","Message")


# Start loop for subscribing and displaying the Message
mqttClient.loop_start() # Start loop for subscribing and displaying the Message
mqttClient.subscribe("example")
time.sleep(50)
mqttClient.loop_end()
mqttClient.disconnect()
print("Disconnected")
