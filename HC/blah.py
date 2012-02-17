#!/usr/bin/env python
# -*- coding: utf-8 -*-

import vtk
import ctypes

from isis import core
from isis import data


# Vista Reader
vista = ctypes.CDLL("isisReader.so")

image = data.IOFactory.load("data/norm024n.v")
print image[0].getProperty("indexOrigin")[0]
print image[0].getVoxel(30,0,20,0)

bild = vista.getImageData(image)



