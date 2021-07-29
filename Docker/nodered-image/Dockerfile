FROM nodered/node-red:latest

# Copy package.json to the WORKDIR so npm builds all
# of your added nodes modules for Node-RED
#COPY package.json .
RUN npm install --unsafe-perm --no-update-notifier --no-fund --only=production

# Copy _your_ Node-RED project files into place
# NOTE: This will only work if you DO NOT later mount /data as an external volume.
#       If you need to use an external volume for persistence then
#       copy your settings and flows files to that volume instead.
#COPY settings.js /data/settings.js
#COPY flows_cred.json /data/flows_cred.json
#COPY flows.json /data/flows.json

# You should add extra nodes via your package.json file but you can also add them here:
WORKDIR /usr/src/node-red
#!/bin/bash
RUN npm install node-red-admin
RUN npm install node-red-contrib-advanced-ping
RUN npm install node-red-contrib-aggregator
RUN npm install node-red-contrib-azure-sql
RUN npm install node-red-contrib-bigtimer
RUN npm install node-red-contrib-boolean-logic
RUN npm install node-red-contrib-color-convert
RUN npm install node-red-contrib-cpu
RUN npm install node-red-contrib-dashboard-average-bars
RUN npm install node-red-contrib-dashboard-bar-chart-data
RUN npm install node-red-contrib-dashboard-sum-bars
RUN npm install node-red-contrib-googlehome-actions-v2
RUN npm install node-red-contrib-ibm-watson-iot
RUN npm install node-red-contrib-influxdb
RUN npm install node-red-contrib-message-counter
RUN npm install node-red-contrib-msg-resend
RUN npm install node-red-contrib-mssql-plus
RUN npm install node-red-contrib-mssql-ps
RUN npm install node-red-contrib-net-ping
RUN npm install node-red-contrib-nora
RUN npm install node-red-contrib-openhab-v2
RUN npm install node-red-contrib-play-audio
RUN npm install node-red-contrib-plex
RUN npm install node-red-contrib-plex-ws
RUN npm install node-red-contrib-redis
RUN npm install node-red-contrib-ui-led
RUN npm install node-red-contrib-ui-level
RUN npm install node-red-contrib-ui-state-trail
RUN npm install node-red-contrib-vcgencmd
RUN npm install node-red-dashboard
RUN npm install node-red-node-discord
RUN npm install node-red-node-ledborg
RUN npm install node-red-node-pi-gpio
RUN npm install node-red-node-ping
RUN npm install node-red-node-random
RUN npm install node-red-node-rbe
RUN npm install node-red-node-serialport
RUN npm install node-red-node-smooth
RUN npm install node-red-node-tail
RUN npm install node-red-node-wol
RUN npm install node-red-contrib-telegrambot-home
RUN npm install node-red-contrib-whatsapp-cmb
RUN npm install node-red-contrib-azure-blob-storage
RUN npm install node-red-node-pi-gpiod
