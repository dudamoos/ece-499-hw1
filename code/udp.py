#!/usr/bin/python

import sys
if sys.version_info >= (3, 0):
	from functools import reduce

import socket
import array
import struct

class UdpSocket(object):
	_len_pack = struct.Struct('<I')
	_send_big_chunk = 32768 # ~half of max datagram size
	
	def __init__(self, recv_addr, receiving = False):
		self.addr = recv_addr
		self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
		if (receiving): self.sock.bind(self.addr)
	def __del__(self):
		self.sock.close()
	def set_send_bufsize(self, size): self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_SNDBUF, size)
	def set_recv_bufsize(self, size): self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_RCVBUF, size)
	def set_send_bufsize_big(self): self.set_send_bufsize(UdpSocket._send_big_chunk + 2000)
	def set_recv_bufsize_big(self): self.set_recv_bufsize(UdpSocket._send_big_chunk + 2000)
	
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
		# Don't do validation on same address to allow flowing through NAT
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
	
	def send_big(self, data):
		data = array.array('B', data)
		total_len = len(data)
		total_sent = 0
		self.send(UdpSocket._len_pack.pack(total_len))
		while total_sent < total_len:
			chunk_len = min(total_len - total_sent, UdpSocket._send_big_chunk)
			chunk = data[total_sent : total_sent + chunk_len]
			self.send(chunk)
			total_sent += chunk_len
	
	def recv_big(self):
		length = self.recv(UdpSocket._len_pack.size, require_full=True)
		if length == None: return None # Corrupted data
		length = UdpSocket._len_pack.unpack(length)[0]
		buf = array.array('B')
		while length > 0:
			packet = self.recv(min(length, UdpSocket._send_big_chunk), require_full=True)
			if packet == None: return None # Corrupted data
			buf.extend(packet)
			length -= len(packet)
		return buf
