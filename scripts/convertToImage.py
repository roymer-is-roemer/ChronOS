#!/usr/bin/python3

import sys
import ctypes
from PIL import Image

if len(sys.argv) < 3:
    print("Usage: convertToImage.py <inputFileName> <outputFileName>")
    exit(1)

inFile = sys.argv[1]
outFile = sys.argv[2]


pil_image = Image.open(inFile).convert('RGB')
pixels = pil_image.tobytes()

with open(outFile, "wb") as f:
    f.write(ctypes.c_uint16(pil_image.width))
    f.write(ctypes.c_uint16(pil_image.height))
    f.write(pixels)
