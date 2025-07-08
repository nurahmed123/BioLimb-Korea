# === Python: Mouth Open Detection + IK + Serial Control (Saliva Sampling) ===

import cv2
import mediapipe as mp
import math
import time
import os
from serialCommunication import sendCmd
from playsound import playsound
from config import mouthOpenThreshold

# === Configuration ===
L1 = 100  # Length of first arm segment in mm
L2 = 100  # Length of second arm segment in mm
sampling_started = False
sampling_cooldown = 3  # seconds between attempts
last_sample_time = 0

# === MediaPipe Setup ===
mp_face_mesh = mp.solutions.face_mesh
mp_drawing = mp.solutions.drawing_utils
cap = cv2.VideoCapture(0)
mouthClosePlay = 1

# === Inverse Kinematics Function ===
def inverse_kinematics(x, y):
    try:
        r = math.sqrt(x**2 + y**2)
        if r > (L1 + L2):
            return None

        cos_theta2 = (r**2 - L1**2 - L2**2) / (2 * L1 * L2)
        theta2 = math.acos(cos_theta2)

        angle_a = math.atan2(y, x)
        angle_b = math.acos((L1**2 + r**2 - L2**2) / (2 * L1 * r))
        theta1 = angle_a - angle_b
        theta3 = 0  # Optional gripper control

        return math.degrees(theta1), math.degrees(theta2), math.degrees(theta3)
    except:
        return None

# === Mouth Open Check (scaled for threshold) ===
def calculate_mouth_distance(landmarks):
    top_lip = landmarks[13]
    bottom_lip = landmarks[14]
    dist = math.sqrt((bottom_lip.x - top_lip.x)**2 + (bottom_lip.y - top_lip.y)**2)
    return dist * 1000

# === Get mouth position ===
def get_mouth_position(landmarks, w, h):
    mouth = landmarks[14]  # bottom lip
    x = int(mouth.x * w)
    y = int(mouth.y * h)
    z = mouth.z
    return x, y, z

# === Main Loop ===
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
                mouth_dist = calculate_mouth_distance(face_landmarks.landmark)

                if mouth_dist > mouthOpenThreshold:
                    x, y, z = get_mouth_position(face_landmarks.landmark, w, h)
                    x_mm = (x - w / 2) * 0.5
                    y_mm = (y - h / 2) * 0.5

                    ik_result = inverse_kinematics(x_mm, y_mm)
                    if ik_result:
                        t1, t2, t3 = ik_result
                        current_time = time.time()

                        if not sampling_started or (current_time - last_sample_time) > sampling_cooldown:
                            sendCmd(f"{int(t1)},{int(t2)},{int(t3)}\n")
                            time.sleep(0.5)  # Let the arm position
                            sendCmd("PUSH\n")  # Push cotton bud
                            time.sleep(1.5)     # Sample delay
                            sendCmd("PULL\n")  # Pull out
                            sampling_started = True
                            last_sample_time = current_time

                    cv2.putText(image, "Mouth OPEN - Sampling", (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 2)
                    print("MO")
                else:
                    cv2.putText(image, "Mouth CLOSED", (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)
                    sendCmd("MC\n")
                    if mouthClosePlay:
                        try:
                            sound_path = os.path.join(os.path.dirname(__file__), "open.mp3")
                            playsound(sound_path, True)
                        except Exception as e:
                            print(f"Sound error: {e}")
                        mouthClosePlay = 0
                    sampling_started = False
                    print("MC")

        cv2.imshow("Mouth Sampling Robot", image)
        if cv2.waitKey(5) & 0xFF == 27:
            break

cap.release()
cv2.destroyAllWindows()
