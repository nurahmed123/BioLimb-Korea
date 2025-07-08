import cv2
import mediapipe as mp
import math
from serialCommunication import sendCmd
import time

# Robot Arm Lengths in mm
L1 = 100
L2 = 100

# Initialize MediaPipe
mp_face_mesh = mp.solutions.face_mesh
cap = cv2.VideoCapture(0)

# Inverse Kinematics
def inverse_kinematics(x, y):
    try:
        r = math.sqrt(x ** 2 + y ** 2)
        if r > (L1 + L2):
            return None

        cos_theta2 = (r ** 2 - L1 ** 2 - L2 ** 2) / (2 * L1 * L2)
        theta2 = math.acos(cos_theta2)

        angle_a = math.atan2(y, x)
        angle_b = math.acos((L1 ** 2 + r ** 2 - L2 ** 2) / (2 * L1 * r))

        theta1 = angle_a - angle_b
        theta3 = 0

        return math.degrees(theta1), math.degrees(theta2), math.degrees(theta3)
    except:
        return None

# Get nostril landmark position
def get_nostril_position(landmarks, w, h):
    nostril = landmarks[98]
    return int(nostril.x * w), int(nostril.y * h), nostril.z

# Main loop
with mp_face_mesh.FaceMesh(max_num_faces=1) as face_mesh:
    while cap.isOpened():
        success, image = cap.read()
        if not success:
            continue

        h, w, _ = image.shape
        rgb = cv2.cvtColor(cv2.flip(image, 1), cv2.COLOR_BGR2RGB)
        results = face_mesh.process(rgb)
        image = cv2.cvtColor(rgb, cv2.COLOR_RGB2BGR)

        if results.multi_face_landmarks:
            for face_landmarks in results.multi_face_landmarks:
                x, y, z = get_nostril_position(face_landmarks.landmark, w, h)

                # Convert image coordinates to mm (calibrated scale)
                x_mm = (x - w / 2) * 0.5
                y_mm = (y - h / 2) * 0.5

                ik = inverse_kinematics(x_mm, y_mm)
                if ik:
                    t1, t2, t3 = ik
                    payload = f"{int(t1)},{int(t2)},{int(t3)}\n"
                    sendCmd(payload)  # send data to mcu
                # Visual cue
                cv2.circle(image, (x, y), 5, (255, 255, 0), -1)

        cv2.imshow("Nose Tracker", image)
        if cv2.waitKey(5) & 0xFF == 27:
            break

cap.release()
cv2.destroyAllWindows()
