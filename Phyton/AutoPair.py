#!/usr/bin/python
import logging
import sys
import time
import pexpect
import subprocess

class BtAutoPair:
        """Class to auto pair and trust with bluetooth."""

        def __init__(self):
                p = subprocess.Popen("/usr/local/bin/auto-agent", shell = False)
                out = subprocess.check_output("/usr/sbin/rfkill unblock bluetooth", shell = True)
                self.child = pexpect.spawn("bluetoothctl", echo = False)
                logging.warning('BtAutoPair start')


        def get_output(self,command, pause = 0):
                """Run a command in bluetoothctl prompt, return output as a list of lines."""
                self.child.send(command + "\n")
                time.sleep(pause)
                start_failed = self.child.expect(["bluetooth", pexpect.EOF])

                if start_failed:
                        raise BluetoothctlError("Bluetoothctl failed after running " + command)

                return self.child.before.split("\r\n")

        def enable_pairing(self):
                """Make device visible to scanning and enable pairing."""
                print "pairing enabled"
                logging.warning('Pairing Enabled')

                try:
                        out = self.get_output("power on")
                        out = self.get_output("discoverable on")
                        out = self.get_output("pairable on")
                        out = self.get_output("agent off")

                except BluetoothctlError, e:
                        print(e)
                        return None

        def disable_pairing(self):
                """Disable devices visibility and ability to pair."""
                try:
                        out = self.get_output("discoverable off")
                        out = self.get_output("pairable off")

                except BluetoothctlError, e:
                        print(e)
                        return None


logging.basicConfig(filename='app.log', filemode='w', format='%(name)s - %(levelname)s - %(message)s')
logging.warning('This will get logged to a file 2 ')
logging.basicConfig(level=logging.DEBUG)
autopair = BtAutoPair()
autopair.enable_pairing()
