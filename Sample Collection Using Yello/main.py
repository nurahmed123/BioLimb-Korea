from ultralytics import YOLO
import cv2
import math
import serial
import time

# Serial communication to Arduino (adjust port if needed)
arduino = serial.Serial('/dev/cu.usbmodemF412FA9F73AC2', 9600, timeout=1)
time.sleep(2)  # Wait for Arduino to initialize

# Start webcam
cap = cv2.VideoCapture(0)
cap.set(3, 640)
cap.set(4, 480)

# Load YOLO model
model = YOLO("best.pt")

# Class names
classNames = ["mouth", "orphanyx"]

# Frame center and dead zone
frame_center_x = 640 // 2
frame_center_y = 480 // 2
dead_zone = 30  # pixels

while True:
    success, img = cap.read()
    if not success:
        break

    results = model(img, stream=True)

    for r in results:
        boxes = r.boxes

        for box in boxes:
            x1, y1, x2, y2 = map(int, box.xyxy[0])

            # Draw bounding box
            cv2.rectangle(img, (x1, y1), (x2, y2), (255, 0, 255), 3)

            # Get class and center
            cls = int(box.cls[0])
            class_name = classNames[cls]
            cx = (x1 + x2) // 2
            cy = (y1 + y2) // 2

            # Draw center
            cv2.circle(img, (cx, cy), 5, (0, 255, 0), -1)
            cv2.putText(img, f'{class_name} ({cx},{cy})', (x1, y1 - 10),
                        cv2.FONT_HERSHEY_SIMPLEX, 0.6, (255, 0, 0), 2)

            if class_name == "mouth":
                # Track X-axis
                if cx < frame_center_x - dead_zone:
                    arduino.write(b'XLR50\n')
                    print("Sent: XLR50")
                elif cx > frame_center_x + dead_zone:
                    arduino.write(b'XRR50\n')
                    print("Sent: XRR50")

                # Track Y-axis
                if cy < frame_center_y - dead_zone:
                    arduino.write(b'YLR50\n')
                    print("Sent: YLR50")
                elif cy > frame_center_y + dead_zone:
                    arduino.write(b'YRR50\n')
                    print("Sent: YRR50")

    cv2.imshow("Webcam", img)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Cleanup
cap.release()
cv2.destroyAllWindows()
arduino.close()
