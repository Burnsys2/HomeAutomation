import os
import time
import sys
import paho.mqtt.client as mqtt
import RPi.GPIO as GPIO
import alsaaudio
from bmp280 import BMP280
import psutil

import socket

class HaltException(Exception):
    pass




def measure_temp():
        temp = os.popen("vcgencmd measure_temp").readline()
        return (temp.replace("temp=","").replace("'C",""))

def measure_temp():
        temp = os.popen("vcgencmd measure_temp").readline()




def getRAMinfo():
    p = os.popen('free')
    i = 0
    while 1:
        i = i + 1
        line = p.readline()
        if i==2:
            return(line.split()[1:4])


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
        if topics[2].upper() == "PSCOUNT":
            psCount = os.popen("pgrep " + message.payload + " | wc -l").readline().replace('\r\n', '').rstrip("\r\n")
            client.publish('MXQ/out/PSCOUNT/' + message.payload , psCount, 0)
        if topics[2].upper() == "PSKILL":
            os.popen("pkill -f '" + message.payload + "'")
        if topics[2].upper() == "PSRUN":
            os.popen(message.payload)

    except Exception as ex:
        print ex
        MustEnd = True
        raise HaltException("We decided we want to halt the script")


# Data capture and upload interval in seconds. Less interval will eventually hang the DHT22.
INTERVAL=2
global MustEnd
global Host
os.path.expanduser('~pi')
sensor_data = {'temperature': 0, 'humidity': 0}
print "Iniciando..."
next_reading = time.time() 
MustEnd = False

try: 
    Host =socket.gethostname()
    print(Host)
    client = mqtt.Client()

    client.connect("mqtt.burnsys.ddns.net", 1883, 60)
    client.reconnect_delay_set(2, 10)
    client.subscribe(Host + "/in/#")
    client.on_message=on_message
    try:
        from smbus2 import SMBus
    except ImportError:
        from smbus import SMBus

    bus = SMBus(1)
    bmp280 = BMP280(i2c_dev=bus)
    client.loop_start()

    #sudo apt-get install python-alsaaudio
    audioDevice = alsaaudio.Mixer("Headphone") #

    while True:
        next_reading += INTERVAL

        sleep_time = next_reading-time.time()
        if sleep_time > 0:
            time.sleep(sleep_time)
        
        # Sending humidity and temperature data to ThingsBoard
        RAM_stats = getRAMinfo()
        RAM_total = round(int(RAM_stats[0]) / 1000,1)
        client.publish(Host + '/out/CPU', psutil.cpu_percent())
        client.publish(Host + '/out/RAM/Total', round(int(RAM_stats[0]), 0))
        client.publish(Host + '/out/RAM/Used', round(int(RAM_stats[1]), 0))
        client.publish(Host + '/out/RAM/Free', round(int(RAM_stats[2]), 0))
        client.publish(Host + '/out/volume', str(audioDevice.getvolume()[0]))


#vcgencmd measure_clock arm

 #       vcgencmd get_throttled
#vcgencmd measure_volts

#        client.publish(Host + '/out/Temp', str(measure_temp()))
        client.publish(Host + '/out/Temp', str(os.popen("vcgencmd measure_temp").readline().replace("temp=","").replace("'C","")))
        client.publish(Host + '/out/Throttled', str(os.popen("vcgencmd get_throttled").readline()).replace("throttled=",""))
        client.publish(Host + '/out/Clock', str(os.popen("vcgencmd measure_clock arm").readline()).replace("frequency(48)=",""))
        client.publish(Host + '/out/Volts', str(os.popen("vcgencmd measure_volts").readline()).replace("volt=","").replace("V",""))
        try:
            client.publish(Host + '/out/BMP/Temp', str(bmp280.get_temperature()))
            client.publish(Host + '/out/BMP/Press', str(bmp280.get_pressure()))
        except:
            client.publish(Host + '/out/BMP/Temp', 0)
            client.publish(Host + '/out/BMP/Press', 0)
            bus = SMBus(1)
            bmp280 = BMP280(i2c_dev=bus)
            pass
    
    

        if MustEnd == True:
            sys.exit(os.EX_SOFTWARE) 
            
except HaltException as ex:
    sys.exit(os.EX_SOFTWARE) 

client.loop_stop()
client.disconnect()
