#!/usr/bin/python
from __future__ import print_function

import shared
import udp
import time

transport = udp.UdpSocket(shared.RPI_CMD_ADDR, receiving=True)

while True:
	start = time.time()
	buf = transport.recv(shared.DOFS_BUF_LEN)
	period = time.time() - start
	dofs = shared.unpack_dofs(buf)
	print("Received values:", dofs)
	dofs_doubled = [2*d for d in dofs]
	print("Doubled values:", dofs_doubled)
	print("Bitrate:", 8 * shared.DOFS_BUF_LEN / period, "bits/s")
