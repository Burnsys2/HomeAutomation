import os
import time
import sys
import paho.mqtt.client as mqtt
import psutil

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

def on_message(client, userdata, message):
    topics = message.topic.split("/")
# Data capture and upload interval in seconds. Less interval will eventually hang the DHT22.
INTERVAL=2

next_reading = time.time() 

client = mqtt.Client()

# Set access token

# Connect to ThingsBoard using default MQTT port and 60 seconds keepalive interval
client.connect("192.168.2.1", 1883, 60)
client.reconnect_delay_set(2, 10)
client.subscribe("MXQ/in/#")
client.on_message=on_message

client.loop_start()
try:
    while True:
        # Sending humidity and temperature data to ThingsBoard
        client.publish('MXQ/out/Temp', measure_temp(), 0)
        RAM_stats = getRAMinfo()
        RAM_total = round(int(RAM_stats[0]) / 1000,1)
        client.publish('MXQ/out/CPU', round(int(psutil.cpu_percent(), 0))
        client.publish('MXQ/out/RAM/Total', round(int(RAM_stats[0]), 0))
        client.publish('MXQ/out/RAM/Used', round(int(RAM_stats[1]), 0))
        client.publish('MXQ/out/RAM/Free', round(int(RAM_stats[2]), 0))
         next_reading += INTERVAL
        sleep_time = next_reading-time.time()
        if sleep_time > 0:
            time.sleep(sleep_time)
except KeyboardInterrupt:
    pass

client.loop_stop()
client.disconnect()
