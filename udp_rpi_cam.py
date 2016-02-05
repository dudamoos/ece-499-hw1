#!/usr/bin/python
from __future__ import print_function

import shared
import udp
import time
import cv2

transport = udp.UdpSocket(shared.PC_CAM_ADDR)
image = cv2.imread('robot.bmp')

PERIOD = 1.0/5

while True:
	next_time = time.time() + PERIOD
	transport.send_big(shared.img2bufz(image))
	throttle_delay = next_time - time.time()
	print("Sleep for", throttle_delay, "until", next_time)
	if throttle_delay > 0: time.sleep(throttle_delay)
