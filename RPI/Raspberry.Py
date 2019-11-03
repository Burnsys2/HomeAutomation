import os
import time
import sys
import paho.mqtt.client as mqtt
import RPi.GPIO as GPIO

def getRAMinfo():
    p = os.popen('free')
    i = 0
    while 1:
        i = i + 1
        line = p.readline()
        if i==2:
            return(line.split()[1:4])

def measure_temp():
        temp = os.popen("vcgencmd measure_temp").readline()
        return (temp.replace("temp=","").replace("'C",""))

# Data capture and upload interval in seconds. Less interval will eventually hang the DHT22.
INTERVAL=2

sensor_data = {'temperature': 0, 'humidity': 0}
print "Iniciando..."
next_reading = time.time() 

client = mqtt.Client()

# Set access token

# Connect to ThingsBoard using default MQTT port and 60 seconds keepalive interval
client.connect("192.168.2.1", 1883, 60)
GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
GPIO.setup(5,GPIO.OUT)
GPIO.setup(3,GPIO.OUT)
GPIO.output(5,GPIO.HIGH)
GPIO.output(3,GPIO.HIGH)
GPIO.output(5,GPIO.LOW)
GPIO.output(3,GPIO.LOW)

GPIO.cleanup()
client.loop_start()

try:
    while True:

        # Sending humidity and temperature data to ThingsBoard
        client.publish('RPI/Temp', measure_temp(), 0)
        RAM_stats = getRAMinfo()
        RAM_total = round(int(RAM_stats[0]) / 1000,1)
        client.publish('RPI/RAM/Total', round(int(RAM_stats[0]), 0))
        client.publish('RPI/RAM/Used', round(int(RAM_stats[1]), 0))
        client.publish('RPI/RAM/Free', round(int(RAM_stats[2]), 0))
        next_reading += INTERVAL
        sleep_time = next_reading-time.time()
        if sleep_time > 0:
            time.sleep(sleep_time)
except KeyboardInterrupt:
    pass

client.loop_stop()
client.disconnect()
