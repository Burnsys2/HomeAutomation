import os
import time
import sys
import paho.mqtt.client as mqtt
import psutil

class HaltException(Exception):
    pass

def getRAMinfo():
    p = os.popen('free')
    i = 0
    while 1:
        i = i + 1
        line = p.readline()
        if i==2:
            return(line.split()[1:4])

def measure_temp():
        temp = os.popen("cat /sys/class/thermal/thermal_zone0/temp").readline()
        return (temp.replace("temp=","").replace("'C",""))
def start_node():
    global NodeRunning
    if NodeRunning == False:
        print "START"
        os.popen("pm2 start /usr/bin/node-red -- -v")
        NodeRunning = True
def stop_node():
    global NodeRunning
    global LastStop
    LastStop = 0
    if NodeRunning == True:
        print "FINISH"    
        os.popen('pm2 stop /usr/bin/node-red')
        NodeRunning = False

def on_message(client, userdata, message):
    global MustEnd
    global NodeRunning
    try:
        topics = message.topic.split("/")
        if topics[2].upper() == "RESET":
            os.popen("shutdown -r -t 0")
        if topics[2].upper() == "NODESTOPFORCE":
            NodeRunning = True
            stop_node()
        if topics[2].upper() == "NODESTOP":
            stop_node()
        if topics[2].upper() == "NODESTART":
            start_node()
        if topics[2].upper() == "FAIL":
            print "FAIL"
            raise HaltException("We decided we want to halt the script")
    except Exception as ex:
        print ex
        MustEnd = True
        raise HaltException("We decided we want to halt the script")


def on_connect(client, userdata, flags, rc):
     # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe("MXQ/in/#")


# Data capture and upload interval in seconds. Less interval will eventually hang the DHT22.
INTERVAL=2
next_reading = time.time() 
os.path.expanduser('~root')
global MustEnd
global LastStop
global NodeRunning
LastStop = 0
MustEnd = False
NodeRunning = True
stop_node()

try:
    client = mqtt.Client()

    # Set access token

    # Connect to ThingsBoard using default MQTT port and 60 seconds keepalive interval
    client.connect("mqtt.burnsys.ddns.net", 1883, 60)
    client.reconnect_delay_set(2, 10)
    client.on_connect = on_connect
    client.on_message=on_message
    client.loop_start()

    while True:
        next_reading += INTERVAL
        sleep_time = next_reading-time.time()
        if sleep_time > 0:
            time.sleep(sleep_time)
        
        # Sending humidity and temperature data to ThingsBoard
        client.publish('MXQ/out/Temp', measure_temp(), 0)
        RAM_stats = getRAMinfo()
        RAM_total = round(int(RAM_stats[0]) / 1000,1)
        client.publish('MXQ/out/CPU', psutil.cpu_percent())
        client.publish('MXQ/out/RAM/Total', round(int(RAM_stats[0]), 0))
        client.publish('MXQ/out/RAM/Used', round(int(RAM_stats[1]), 0))
        client.publish('MXQ/out/RAM/Free', round(int(RAM_stats[2]), 0))

        if MustEnd == True:
            sys.exit(os.EX_SOFTWARE) 
        if NodeRunning == False:
            LastStop = LastStop + 1
        if LastStop >= 5:
            start_node()

except HaltException as ex:
    sys.exit(os.EX_SOFTWARE) 

client.loop_stop()
client.disconnect()

