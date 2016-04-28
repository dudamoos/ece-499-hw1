#!/usr/bin/python -O
from __future__ import print_function

import shared, udp, time
from picamera.array import PiRGBArray
from picamera import PiCamera
import cv2

transport = udp.UdpSocket(shared.PC_CAM_ADDR)
with PiCamera(resolution=(320, 240), framerate=10) as camera:
	raw_cap = PiRGBArray(camera, size=(320, 240))
	time.sleep(2)
	for frame in camera.capture_continuous(raw_cap, format="bgr", use_video_port=True):
		transport.send_big(shared.img2bufz(raw_cap.array))
		raw_cap.truncate(0)
		print("Sent a frame!")
