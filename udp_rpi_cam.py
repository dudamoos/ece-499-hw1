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
	start = time.time()
	buf = transport.send(shared.img2buf(image))
	time.sleep(start + PERIOD - time.time())
