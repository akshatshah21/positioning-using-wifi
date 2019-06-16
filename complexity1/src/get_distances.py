#!python3
#Header files

import paho.mqtt.client as mqtt
import time
import os

'''
For some reason, this function doesn't work (No error displayed)
# On message function for displaying the message 'on_message'
def on_message(client, userdata, message):
	print("Message:" + str(message.payload.decode(utf-8)))
'''


#Initialize counter for trifurcating the distances in order
count = 0
print("count set to 1")


# On message function for displaying and storing the message
def on_message(client, userdata, message):
    # global count #Note this. To change a global variable
    print(message.topic, message.payload.decode())
    if str(message.topic) == 'numOfTags':
        n = int(message.payload.decode())
        f = []
        for i in range (0,n):
            f[i] = open(str(i),'w')
		# global count
		count = 1 				# Maybe could add a flag that numOfTags is received and corresponding files created
    elif count % 3 == 1:  #Check
        print("Entered count 1 case") #For debugging
        if str(message.topic) == 'd1':
            global count
            print("Printing to Distance1.txt")
            f1.write(message.payload.decode() + '\n')
            f1.flush()
            # typically the above line would do. however this is used to ensure that the file is written
            os.fsync(f1.fileno())
            count += 1
            # print("count=",count)
    elif count % 3 == 2:   #Check
        print("Entered count 2 case")   #For debugging
        if message.topic == 'd2':
            global count
            print("Printing to Distance2.txt")
            f2.write(message.payload.decode() + '\n')
            f2.flush()
            # typically the above line would do. however this is used to ensure that the file is written
            os.fsync(f2.fileno())
            count += 1
            print("Count set to 3")
    elif count % 3 == 0:
        print("Entered count 3 case")    #For debugging
        if message.topic == 'd3':
            global count
            print("Printing to Distance3.txt")
            f3.write(message.payload.decode() + '\n')
            f3.flush()
            # typically the above line would do. however this is used to ensure that the file is written
            os.fsync(f3.fileno())
            count += 1


broker = "postman.cloudmqtt.com" #Hostname. May be IP address also

# The client name is used by the MQTT broker to identify the client.
mqttClient = mqtt.Client("python1",True)


# Attaching on_message function
mqttClient.on_message = on_message


# Set username and pwd for CloudMQTT Auth
mqttClient.username_pw_set(username = "ocbshoyv",password = "u7RF9Xts1g1r")


#Connect to broker
print("Connecting to broker:"+broker)
mqttClient.connect(broker,16557) # Param 1: broker, Param 2:Port Number
print("Connected")


#Publish a message 'message' to 'example' topic
# mqttClient.publish("example","Message")

#Subscribe to topic "example', d1,d2,d3

mqttClient.subscribe([("a1",0),("a2",0),("a3",0)])

f1 = open("Tag1/Distance1.txt",'w')
f2 = open("Tag2/Distance2.txt",'w')
f3 = open("Distance3.txt",'w')
mqttClient.loop_forever()


# f1.close()
# f2.close()
# mqttClient.loop_forever()
# f1.close()
# f2.close()
# f3.close()

# mqttClient.loop_forever();
'''
# Start loop for subscribing and displaying the Message
mqttClient.loop_start() # Start loop for subscribing and displaying the Message
time.sleep(50)
mqttClient.loop_end()

mqttClient.disconnect()
print("Disconnected")
'''
