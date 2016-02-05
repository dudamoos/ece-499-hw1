import struct
import numpy

import secret

RPI_CMD_ADDR = (secret.RPI_HOST, secret.CMD_PORT)
PC_CAM_ADDR = (secret.PC_HOST, secret.CAM_PORT)

NUM_DOFS = 2
DOFS_STRUCT = struct.Struct('<' + str(NUM_DOFS) + 'f')
DOFS_BUF_LEN = DOFS_STRUCT.size

def pack_dofs(*args): return DOFS_STRUCT.pack(*args)
def unpack_dofs(buf): return DOFS_STRUCT.unpack(buf)

IMG_SIZE = 224532 # cv2.imread('robot.bmp').size
IMG_SHAPE = (297, 252, 3)
IMG_DTYPE = numpy.uint8

def img2buf(img): return img.tostring()
def buf2img(buf): return numpy.frombuffer(buf, IMG_DTYPE).reshape(IMG_SHAPE)
