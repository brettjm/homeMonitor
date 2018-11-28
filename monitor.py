import time
import picamera
import numpy as np

with picamera.PiCamera() as camera:
   camera.resolution = (320, 240)
   camera.framerate = 24
   time.sleep(2)
   image = np.empty((240 * 320 * 3), dtype=np.uint8)
   camera.capture(image, 'bgr')
   
   # np.savetxt('test.txt', image)