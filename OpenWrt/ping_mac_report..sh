#!/bin/sh

while sleep 5; do (ping_mac.sh $1 &) ; done
