#!python3

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


count = 1
def on_message(client, userdata, message):
    print("Message received")
    global count #Note this. To change a global variable
    print(message.topic, message.payload.decode())
    if count % 3 == 1:  #Check
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

mqttClient.subscribe([("d1",0),("d2",0),("d3",0)])

f1 = open("Distance1.txt",'w')
f2 = open("Distance2.txt",'w')
f3 = open("Distance3.txt",'w')
mqttClient.loop_forever()



# setup_mqtt()
# mqttClient.loop_start()
# print("in loop")
# time.sleep(15)
# mqttClient.loop_stop()
mqttClient.loop_forever()

print("After loop")
