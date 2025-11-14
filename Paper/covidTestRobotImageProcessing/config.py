RASPBERRY = False  #True or False


human_detect_threshold = 20 #cm


mouthOpenThreshold = 80  # change this value to adjust mouth open/close logic

# distance from camera to object(face) measured
# centimeter
Known_distance = 28 #18     76.2

# width of face in the real world or Object Plane
# centimeter
Known_width = 14.3  #14.3


human_detect_flag = True
mouth_open_flag = 0

# Tracking parameters (pixel-to-step mapping and limits)
PIXELS_TO_STEPS_X = 1.0
PIXELS_TO_STEPS_Y = 1.0
TRACKING_DEADBAND_PIXELS = 8
MAX_STEP_PER_UPDATE = 15
Z_APPROACH_STEPS_PER_ITER = 5
MAX_Z_APPROACH_STEPS = 120

