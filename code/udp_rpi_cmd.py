#!/usr/bin/python -O
from __future__ import print_function

import shared
import udp
import serial
import time, sys, os

# find device to write to
device = shared.get_serial(sys.argv[1] if len(sys.argv) > 1 else None)
if device == None: exit(1)
baud = int(sys.argv[2]) if len(sys.argv) > 2 else 9600
print("Using OpenCM via", device, "@", baud, "baud")
usb = serial.Serial(device, baud)

transport = udp.UdpSocket(shared.RPI_CMD_ADDR, receiving=True)
transport.set_recv_bufsize(8)

while True:
	cmd = transport.recv(1).tostring()
	#print("Received command: '", cmd, "' @ ", time.time(), sep='')
	usb.write(cmd)

