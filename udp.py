#!/usr/bin/python

import sys
if sys.version_info >= (3, 0):
	from functools import reduce

import socket
import array

class UdpSocket(object):
	def __init__(self, recv_addr, receiving = False):
		self.addr = recv_addr
		self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
		if (receiving): self.sock.bind(self.addr)
	def __del__(self):
		self.sock.close()
	
	def send(self, data):
		data = array.array('B', data)
		cksum = ~reduce(lambda x,y: x+y, data) & 0xFF
		packet = bytearray(data) + bytearray([cksum])
		sent = self.sock.sendto(packet, self.addr)
		while sent < len(packet):
			assert sent > 0, "Network unavailable"
			packet = packet[sent-1:]
			sent = self.sock.sendto(packet, self.addr)
	
	def recv(self, length, require_full = True):
		# packet should always end with checksum, but it might be shorter than *length* if !require_full
		# Could do validation on same address, but that's probably overkill here
		remaining = length + 1
		packet, address = self.sock.recvfrom(remaining)
		assert len(packet) > 0, "Network unavailable"
		remaining -= len(packet)
		if require_full:
			while remaining > 0:
				buf, address = self.sock.recvfrom(remaining)
				assert len(buf) > 0, "Network unavailable"
				remaining -= len(buf)
				packet.extend(buf)
		
		packet = array.array('B', ''.join(packet))
		data = packet[:-1]
		cksum = packet[-1]
		if (cksum + reduce(lambda x,y: x+y, data) & 0xFF) != 0xFF:
			return None # Corrupted data
		return data
