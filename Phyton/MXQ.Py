import os
import time
import sys
import paho.mqtt.client as mqtt
import psutil
import socket

class HaltException(Exception):
    pass

def measure_temp():
        temp = os.popen("cat /sys/class/thermal/thermal_zone0/temp").readline()
        return (temp.replace("temp=","").replace("'C",""))

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
        if topics[2].upper() == "FAIL":
            print "FAIL"
            raise HaltException("We decided we want to halt the script")
        if topics[2].upper() == "PSCOUNT":
            psCount = os.popen("pgrep " + message.payload + " | wc -l").readline().replace('\r\n', '').rstrip("\r\n")
            client.publish(Host + '/out/PSCOUNT/' + message.payload , psCount, 0)
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

    client.loop_start()

    #sudo apt-get install python-alsaaudio
    client.publish(Host + '/out/START',1)
 
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
        if os.path.exists('/mnt/hdd/mountok.txt'):
            client.publish(Host + '/out/HDD/Ok', 1, 0)
            hdd = psutil.disk_usage('/mnt/hdd')
            client.publish(Host + '/out/HDD/Free',str(hdd.free / (2**30)) , 0)
        else:
            client.publish(Host + '/out/HDD/Ok', 0, 0)


#vcgencmd measure_clock arm

 #       vcgencmd get_throttled
#vcgencmd measure_volts

        client.publish(Host + '/out/Temp', str(measure_temp()))
  

        if MustEnd == True:
            sys.exit(os.EX_SOFTWARE) 
            
except HaltException as ex:
    sys.exit(os.EX_SOFTWARE) 

client.loop_stop()
client.disconnect()
