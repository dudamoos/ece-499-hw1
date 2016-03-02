from __future__ import print_function

import struct, os, sys, numpy

import secret
RPI_CMD_ADDR = (secret.RPI_HOST, secret.CMD_PORT)
PC_CAM_ADDR = (secret.PC_HOST, secret.CAM_PORT)

# DOF packing from early on
NUM_DOFS = 2
DOFS_STRUCT = struct.Struct('<' + str(NUM_DOFS) + 'f')
DOFS_BUF_LEN = DOFS_STRUCT.size

def pack_dofs(*args): return DOFS_STRUCT.pack(*args)
def unpack_dofs(buf): return DOFS_STRUCT.unpack(buf)

# Serial device loading - call as get_serial(sys.argv[1] if len(sys.argv) > 1 else None)
def get_serial(device=None):
	if (device != None):
		if os.access(device, os.W_OK): return device
		print("Warning: Cannot write to chosen device - trying default devices", file=sys.stderr)
	if os.access('/dev/ttyACM0', os.W_OK): return '/dev/ttyACM0'
	if os.access('/dev/ttyACM1', os.W_OK): return '/dev/ttyACM1'
	print("Error: Default devices not accessible. Please specify device.", file=sys.stderr)
	return None
