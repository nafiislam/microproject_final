import numpy as np
import os
import cv2
import urllib.request

filename_mp4 = 'survillance.mp4'
frames_per_second = 5.0 # for lower res use larger fps
res = '720p'
url='http://192.168.43.245/cam-hi.jpg'
print('-----------------------------------------------------\n')
i=0
# Video Encoding, might require additional installs
# Types of Codes: http://www.fourcc.org/codecs.php
VIDEO_TYPE = {
    #'mp4': cv2.VideoWriter_fourcc(*'H264'),
    'mp4': cv2.VideoWriter_fourcc(*'XVID'),
}

def get_video_type(filename):
    filename, ext = os.path.splitext(filename)
    if ext in VIDEO_TYPE:
      return  VIDEO_TYPE[ext]

out_mp4 = cv2.VideoWriter(filename_mp4, get_video_type(filename_mp4), frames_per_second, (800,600))

while True:
    img_resp = urllib.request.urlopen(url)
    imgnp = np.array(bytearray(img_resp.read()), dtype=np.uint8)
    img = cv2.imdecode(imgnp, -1)
    frame = img


    out_mp4.write(frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
    if cv2.waitKey(1) & 0xFF == ord('c'):
        cv2.imwrite('capture/capture'+str(i)+'.jpg',frame)
        i=i+1
    cv2.imshow('Surveillance',frame)
    

out_mp4.release()
cv2.destroyAllWindows()
