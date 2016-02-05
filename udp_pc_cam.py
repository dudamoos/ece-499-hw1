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
	buf = transport.recv_big()
	period = time.time() - start
	print("Bitrate:", 8 * len(buf) / period, "bits/s")
	img = shared.buf2img(buf)
	cv2.imshow('imwindow', img)
	cv2.waitKey(10)
	
	new_last = time.time()
	print("Receive Rate:", 1/(new_last - last), "Hz")
	last = new_last
