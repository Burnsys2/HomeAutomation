#!/bin/sh

while sleep 2; do (ping.sh $1 &) ; done
