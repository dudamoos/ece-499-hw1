#!/usr/bin/python
from __future__ import print_function

import shared
import udp
import random
import time

transport = udp.UdpSocket(shared.RPI_CMD_ADDR)
random.seed()

while True:
	dof1 = random.uniform(-1.0, 1.0)
	dof2 = random.uniform(-1.0, 1.0)
	buf = shared.pack_dofs(dof1, dof2)
	transport.send(buf)
	time.sleep(random.random())
