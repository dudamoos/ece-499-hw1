#!/usr/bin/python -O
from __future__ import print_function

import shared
import udp
import serial
import time, sys, os

# find device to write to
device = '/dev/ttyACM0'
if len(sys.argv) > 1:
	device = sys.argv[1]
	if not os.access(device, os.W_OK):
		print("Error: Cannot write to device.")
		exit(1)
elif not os.access(device, os.W_OK):
	device = '/dev/ttyACM1'
	if not os.access(device, os.W_OK):
		print("Error: Default devices not accessible. Please specify device.")
		exit(1)

baud = 9600
if len(sys.argv) > 2: baud = int(sys.argv[2])
print("Using OpenCM via", device, "@", baud, "baud")
usb = serial.Serial(device, baud)

transport = udp.UdpSocket(shared.RPI_CMD_ADDR, receiving=True)
transport.set_recv_bufsize(8)

while True:
	cmd = transport.recv(1).tostring()
	#print("Received command: '", cmd, "' @ ", time.time(), sep='')
	usb.write(cmd)

