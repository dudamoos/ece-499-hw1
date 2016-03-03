#!/usr/bin/python -O
from __future__ import print_function
import time, sys, serial, numpy, cv2, shared
import shared.camera as cam
import shared.pid as pid

PERIOD = 1.0/10
# Determine the desired mask by color
make_mask = cam.create_mask_blue
if len(sys.argv) > 1:
	if sys.argv[1].lower() == 'green': make_mask = cam.create_mask_green
	elif sys.argv[1].lower() == 'blue': make_mask = cam.create_mask_blue
	elif sys.argv[1].lower() == 'red': make_mask = cam.create_mask_red
	else: print("Warning: Unknown object color - assuming blue", file=sys.stderr)

# Create channel to OpenCM
device = shared.get_serial(sys.argv[2] if len(sys.argv) > 2 else None)
if device == None: exit(1)
baud = int(sys.argv[3]) if len(sys.argv) > 3 else 9600
print("Using OpenCM via", device, "@", baud, "baud")
usb = serial.Serial(device, baud)

# Create video capture
capture = cv2.VideoCapture(0)
#cv2.namedWindow('Cam Viewer')
#cv2.namedWindow('Object Detect')
#for i in range(3): cv2.waitKey(10)
#import shared.udp as udp
#transport = udp.UdpSocket(shared.PC_CAM_ADDR)

# Create controller - target, Kp, Ki, Kd, bound, cur_time, window_size
pid_x = pid.PidController(0, 0.4, 0.05, 0.1, 1.0, time.time(), 6)

try:
	while True:
		next_time = time.time() + PERIOD
		
		success, image = capture.read()
		if not success:
			print("\x1b[0KWarning: Couldn't read video frame!", file=sys.stderr)
			continue
		image = cv2.resize(image, (cam.IMG_WIDTH, cam.IMG_HEIGHT))
		mask = make_mask(image)
		x, y, onscreen = cam.find_object_center(mask)
		if onscreen:
			print("\x1b[0KOnscreen @", (x, y))
			cam.draw_marker(mask, x, y, 0x7F)
			# control
			ctl = pid_x.control(x, time.time())
			vel = abs(int(ctl * 650))
			dir = 'j' if ctl < 0 else 'l' if ctl > 0 else ' '
			cmd = dir + str(vel)
			print("\x1b[0KDirection byte:", dir, ", Velocity:", vel)
		else:
			print("\x1b[0KOffscreen ... searching - spinning left\n\x1b[0K")
			print("\x1b[0KDirection byte: j , Velocity: 400")
			cmd = 'j400' # pivot left at speed 400 to search
		usb.write(cmd)
		print("\r\x1b[4A", end='')
		
		#cv2.imshow('Cam Viewer', image)
		#cv2.imshow('Object Detect', mask)
		#cv2.waitKey(10)
		
		#transport.send_big(cam.img2bufz(mask))
		throttle_delay = next_time - time.time()
		if throttle_delay > 0: time.sleep(throttle_delay)
except KeyboardInterrupt:
	capture.release()
	cv2.destroyWindow('Camera Viewer')
	cv2.destroyWindow('Object Tracker')
	print("\n\n\n")

