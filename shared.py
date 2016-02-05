from __future__ import print_function

import cv2
if cv2.__version__.startswith('$Rev'):
	print("OpenCV 2.4 is required")
	raise ImportError("cv2.imencode has the incorrect signature")

import struct
import array
import numpy

import secret

RPI_CMD_ADDR = (secret.RPI_HOST, secret.CMD_PORT)
PC_CAM_ADDR = (secret.PC_HOST, secret.CAM_PORT)

NUM_DOFS = 2
DOFS_STRUCT = struct.Struct('<' + str(NUM_DOFS) + 'f')
DOFS_BUF_LEN = DOFS_STRUCT.size

def pack_dofs(*args): return DOFS_STRUCT.pack(*args)
def unpack_dofs(buf): return DOFS_STRUCT.unpack(buf)

IMG_SHAPE = (297, 252, 3)
IMG_DTYPE = numpy.uint8
IMGZ_QUALITY = 50 # Jpeg quality from 0 to 100 (higher is better but bigger)

def img2buf(img): return img.tostring()
def buf2img(buf): return numpy.frombuffer(buf, IMG_DTYPE).reshape(IMG_SHAPE)
def img2bufz(img): return cv2.imencode(".jpeg", img, (cv2.IMWRITE_JPEG_QUALITY, IMGZ_QUALITY))[1].tostring()
def bufz2img(buf): return cv2.imdecode(numpy.frombuffer(buf, IMG_DTYPE), cv2.IMREAD_COLOR)
