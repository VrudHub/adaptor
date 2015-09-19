# screen captures into a opencv image

import numpy as np
import PIL
import cv2

def getScreen():
    i_pil = PIL.ImageGrab.grab()
    return np.array(i_pil.getdata(), dtype=uint8).reshape((i_pil.size[1], printscreen_pil.size[0], 3))
