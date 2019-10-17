#!/bin/sh
MQTT_SERVER="192.168.2.1"
MQTT_ID_EVENT="OpenWRT-Presence-Event"
MQTT_ID_LASTSEEN="OpenWRT-Ping-LastSeen"
MQTT_TOPIC="OpenWrt/Ping/"

fping -c1 -t300 $1 2>/dev/null 1>/dev/null
if [ "$?" = 0 ]
then
  mosquitto_pub -h $MQTT_SERVER -i $MQTT_ID_LASTSEEN -t "$MQTT_TOPIC$1/lastseen/epoch" -m "$(date +%s)"
fi

