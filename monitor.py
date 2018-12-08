import time
import picamera
import numpy as np

with picamera.PiCamera() as camera:
   camera.resolution = (320, 240)
   camera.framerate = 24
   time.sleep(2)

   # Create empty array to store current image
   image = np.empty((240 * 320 * 3), dtype=np.uint8)

   # Capture the first image in bgr format
   camera.capture(image, 'bgr')
   
   # Initialize estimation arrays
   m_prev = np.empty((240 * 320 * 3), dtype=np.uint8)
   m_curr = np.empty((240 * 320 * 3), dtype=np.uint8)
   o_curr = np.empty((240 * 320 * 3), dtype=np.uint8)
   v_prev = np.empty((240 * 320 * 3), dtype=np.uint8)
   v_curr = np.empty((240 * 320 * 3), dtype=np.uint8)
   e_curr = np.empty((240 * 320 * 3), dtype=np.uint8)
   
   n = 1
   v_min = 2
   v_max = (2**8)-1

   # M of t estimation
   for i, x in enumerate(image):
      if m_prev[i] < x:
         m_curr[i] = m_prev[i] + 1
      elif m_prev[i] > x:
         m_curr[i] = m_prev[i] - 1
      else:
         m_curr[i] = m_prev[i]
      m_prev[i] = m_curr[i]

   # O of t computation
   for i, x in enumerate(image):
      o_curr[i] = np.absolute(m_curr[i] - x)

   # V of t update
   for i, x in enumerate(image):
      if v_prev[i] < n * o_curr[i]:
         v_curr[i] = v_prev[i] + 1
      elif v_prev[i] > n * o_curr[i]:
         v_curr[i] = v_prev[i] - 1
      else:
         v_curr[i] = v_prev[i]
      v_curr[i] = np.maximum(np.minimum(v_curr[i], v_max), vmin)
      v_prev[i] = v_curr[i]

   # E of t estimation
   for i, x in enumerate(image):
      if o_curr[i] < v_curr[i]:
         e_curr[i] = 0
      else:
         e_curr[i] = 1

   # np.savetxt('test.txt', image)