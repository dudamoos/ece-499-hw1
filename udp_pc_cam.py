#!/usr/bin/python
from __future__ import print_function

import shared
import udp
import time
import cv2

transport = udp.UdpSocket(shared.PC_CAM_ADDR, receiving=True)
cv2.namedWindow('imwindow')

last = time.time()
while True:
	start = time.time()
	buf = transport.recv(shared.IMG_SIZE)
	period = time.time() - start
	print("Bitrate:", (shared.IMG_SIZE + 1) / period, "bits/s")
	img = shared.buf2img(buf)
	cv2.imshow('imwindow', img)
	
	new_last = time.time()
	print("Receive Rate:", 1/(new_last - last), "Hz")
	last = new_last
