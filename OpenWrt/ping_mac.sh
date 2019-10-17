#!/bin/sh
MQTT_SERVER="192.168.2.1"
MQTT_ID_EVENT="OpenWRT-Presence-Event"
MQTT_ID_LASTSEEN="OpenWRT-Ping-LastSeen"
MQTT_TOPIC="OpenWrt/Ping/mac-"


#network=192.168.1.1/24
if [ "$#" -ne 1 ]; then echo Usage example: $0 aa:bb:cc:dd:ee:ff; exit 2; fi;
#nmap -sP -T4 $network >& /dev/null
ip=$(arp-scan --localnet | grep $1 | awk ' { print $1 }')
if [ "$ip" = "" ]; then
    echo mac not found 
    exit 1
fi
echo Device \($ip\)
fping -c1 -t300 $ip 2>/dev/null 1>/dev/null
#ping $ip -n -q -c 2 -i 0.2 -w 1 >& /dev/null
#if [ $? -eq 0 ]; then
if [ "$?" = 0 ]; then
    mosquitto_pub -h $MQTT_SERVER -i $MQTT_ID_LASTSEEN -t "$MQTT_TOPIC$1/lastseen/epoch" -m "$(date +%s)"
    echo Device is online \($ip\)
else
    echo Device is offline \($ip\)
    exit 1
fi;
