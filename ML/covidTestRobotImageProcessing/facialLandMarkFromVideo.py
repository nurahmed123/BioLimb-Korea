# Import necessary libraries
from imutils import face_utils
import dlib
import cv2
import math
import time
from config import *
import serialCommunication
from distanceCalculationOfHumanFace import cap
from playsound import playsound

# Initialize camera
cap = cv2.VideoCapture(0)

# Initialize dlib's face detector and predictor
p = "shape_predictor_68_face_landmarks.dat"
detector = dlib.get_frontal_face_detector()
predictor = dlib.shape_predictor(p)

# Timer for reminders
last_reminder_time = time.time()
reminder_interval = 15  # 30 seconds reminder interval
audio_played = False  # Track if the audio has been played

def findFacialLandMark():
    global human_detect_flag, mouth_open_flag, last_reminder_time, audio_played
    result = 0
    _, frame = cap.read()
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    rects = detector(gray, 0)

    for i, rect in enumerate(rects):
        shape = predictor(gray, rect)
        shape = face_utils.shape_to_np(shape)

        flag = 0
        for x, y in shape:
            cv2.circle(frame, (x, y), 2, (0, 255, 0), -1)
            flag += 1
            result = math.sqrt(
                pow(abs(shape[51][0] - shape[57][0]), 2) +
                pow(abs(shape[51][1] - shape[57][1]), 2)
            )

        if result > mouthOpenThreshold:
            cv2.putText(frame, "Mouth Open", (30, 60), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 255, 0), 2)
            mouth_open_flag += 1
            audio_played = False  # Reset the audio reminder flag when the mouth opens
            last_reminder_time = time.time()  # Reset the timer on opening the mouth

            if mouth_open_flag > 15:
                print("Mouth is open long enough.")
            if human_detect_flag:
                serialCommunication.sendCmd("MO\n")
                print("MO")
        else:
            cv2.putText(frame, "Mouth Closed", (30, 60), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 0, 255), 2)
            mouth_open_flag = 0
            print("Mouth is closed")

            if human_detect_flag:
                serialCommunication.sendCmd("MC\n")
                print("MC")

            # Play reminder if 30 seconds have passed since the last reminder and audio hasn't been played
            if not audio_played and (time.time() - last_reminder_time > reminder_interval):
                playsound("open.mp3", True)
                audio_played = True  # Prevent multiple plays until the mouth opens again

    if RASPBERRY:
        pass
    else:
        cv2.imshow("Mouth Detection", frame)

if __name__ == "__main__":
    while True:
        findFacialLandMark()
        key = cv2.waitKey(1)
        if key == 27:
            break

    cap.release()
    cv2.destroyAllWindows()
