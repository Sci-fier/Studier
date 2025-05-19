import cv2
import serial
from picamera2 import Picamera2

def qrdec(room):
    picam2 = Picamera2()
    preview_config = picam2.create_preview_configuration(main={"format": 'XRGB8888', "size": (640, 480)})
    picam2.configure(preview_config)
    picam2.start()
    qr_decoder = cv2.QRCodeDetector()
    try:
        while True:
            frame = picam2.capture_array()
            frame_bgr = cv2.cvtColor(frame, cv2.COLOR_RGBA2BGR)
            value, points, _ = qr_decoder.detectAndDecode(frame_bgr)
            if value==room:
              return "1"
            else:
              return ""
    finally:
        picam2.stop()
      
if name == 'main':
    message=""
    room=""
    ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
    ser.flush()
    while message=="":
      message=ser.readline().decode('utf-8').rstrip ()
    if message[0]=="S":
      room=message[1:]
    while message="":
      message=qrdec(room)
    ser.write (f"{message}\ n" .encode ('utf-8'))
    
