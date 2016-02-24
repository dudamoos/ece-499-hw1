#!/usr/bin/python -Ou
from __future__ import print_function

import shared
import udp
import time
import cv2

transport = udp.UdpSocket(shared.PC_CAM_ADDR, receiving=True)
transport.set_recv_bufsize_big()
cv2.namedWindow('imwindow')
for i in range(3): cv2.waitKey(10) # get the window to appear to start with

sec_start = time.time()
num_frames = 0
while True:
	buf = transport.recv_big()
	if buf == None:
		print("Warning: Couldn't receive video frame!")
		continue
	img = shared.bufz2img(buf)
	if img == None:
		print("Warning: Couldn't decode video frame!")
		continue
	cv2.imshow('imwindow', img)
	cv2.waitKey(10)
	
	num_frames += 1
	loop_end = time.time()
	if (loop_end - sec_start >= 1.0):
		print("Video receive rate: {0:3.2f} Hz".format(
			  num_frames / (loop_end - sec_start)), end='\r')
		num_frames = 0
		sec_start = loop_end
