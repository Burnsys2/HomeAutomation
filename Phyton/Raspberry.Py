import os
import time
import sys
import paho.mqtt.client as mqtt
import RPi.GPIO as GPIO
import alsaaudio

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
        temp = os.popen("vcgencmd measure_temp").readline()
        return (temp.replace("temp=","").replace("'C",""))

def on_message(client, userdata, message):
    global MustEnd
    try:
        topics = message.topic.split("/")
        if topics[2].upper() == "VOLUME":
            audioDevice.setvolume(int(message.payload.decode("utf-8")))
        if topics[2].upper() == "MUTE":
            audioDevice.setvolume(int(not audioDevice.getmute()))
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
    client.subscribe("RPI/in/#")



# Data capture and upload interval in seconds. Less interval will eventually hang the DHT22.
INTERVAL=2
global MustEnd
os.path.expanduser('~pi')
sensor_data = {'temperature': 0, 'humidity': 0}
print "Iniciando..."
next_reading = time.time() 
MustEnd = False
try:
    client = mqtt.Client()

    client.connect("mqtt.burnsys.ddns.net", 1883, 60)
    client.reconnect_delay_set(2, 10)
    client.on_connect = on_connect
    client.on_message=on_message

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
    #sudo apt-get install python-alsaaudio
    audioDevice = alsaaudio.Mixer("PCM") #

    while True:
        # Sending humidity and temperature data to ThingsBoard
        client.publish('RPI/out/Temp', measure_temp(), 0)
        RAM_stats = getRAMinfo()
        RAM_total = round(int(RAM_stats[0]) / 1000,1)
        client.publish('RPI/out/RAM/Total', round(int(RAM_stats[0]), 0))
        client.publish('RPI/out/RAM/Used', round(int(RAM_stats[1]), 0))
        client.publish('RPI/out/RAM/Free', round(int(RAM_stats[2]), 0))
        client.publish('RPI/out/volume', str(audioDevice.getvolume()[0]))
        next_reading += INTERVAL
        sleep_time = next_reading-time.time()
        if sleep_time > 0:
            time.sleep(sleep_time)
        if MustEnd == True:
            sys.exit(os.EX_SOFTWARE) 
            
except HaltException as ex:
    sys.exit(os.EX_SOFTWARE) 

client.loop_stop()
client.disconnect()
