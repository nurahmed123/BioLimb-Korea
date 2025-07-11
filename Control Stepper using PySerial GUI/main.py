import tkinter as tk
from tkinter import Scale, HORIZONTAL, LabelFrame, OptionMenu, Button, Label, StringVar
import serial
import serial.tools.list_ports
import time

# --- Serial Communication Setup ---
ports = serial.tools.list_ports.comports()
available_ports = [port.device for port in ports]

ser = None 

def connect_serial(port_name):
    global ser
    try:
        # Initialize serial instance 
        ser = serial.Serial(port_name, 9600, timeout=1) # Baud rate must match Arduino
        
        status_label.config(text=f"Connected to {port_name}", fg="green")
        print(f"Connected to {port_name}")
        time.sleep(2)
    except serial.SerialException as e:
        status_label.config(text=f"Error: {e}", fg="red")
        print(f"Error connecting to {port_name}: {e}")
    except Exception as e:
        status_label.config(text=f"An unexpected error occurred: {e}", fg="red")
        print(f"An unexpected error occurred during connection: {e}")

def send_command(motor_id, direction, steps):
    if ser and ser.is_open:
        try:
            # Construct the data string: [MotorID][Direction][Steps]
            data_to_send = f"{motor_id}{direction}{steps}"
            # Send data encoded with utf-8 and appended with a newline
            ser.write((data_to_send + "\n").encode("utf-8"))
            print(f"Sent: {data_to_send}\\n") # console logging
        except Exception as e:
            print(f"Error sending command: {e}")
            status_label.config(text=f"Serial Error: {e}", fg="red")
    else:
        status_label.config(text="Not connected to Arduino", fg="orange")
        print("Not connected to Arduino")

# --- Slider Change Handlers ---
# Dictionaries to store previous slider values for each motor
previous_slider_values = {'X': 0, 'Y': 0, 'Z': 0}
slider_range = 500 # Max steps in one direction for slider

def create_slider_handler(motor_id):
    def on_slider_change(val):
        current_value = int(float(val))
        
        # Calculate steps relative to the previous position
        steps_to_move = abs(current_value - previous_slider_values[motor_id])

        if current_value > previous_slider_values[motor_id]: # Moving right (Clockwise)
            if steps_to_move > 0: 
                send_command(motor_id, 'R', steps_to_move)
        elif current_value < previous_slider_values[motor_id]: # Moving left (Counter-clockwise)
            if steps_to_move > 0:
                send_command(motor_id, 'L', steps_to_move)
                
        previous_slider_values[motor_id] = current_value # Update previous value for this motor
    return on_slider_change

# --- GUI Setup ---
app = tk.Tk()
app.title("Individual Stepper Motor Control")

# --- Serial Port Selection ---
port_frame = LabelFrame(app, text="Serial Port Selection")
port_frame.pack(padx=10, pady=5, fill="x")

port_variable = StringVar(app)
if available_ports:
    port_variable.set(available_ports[0]) # Set default to first available port
    port_dropdown = OptionMenu(port_frame, port_variable, *available_ports)
    port_dropdown.pack(side=tk.LEFT, padx=5, pady=5)
    connect_button = Button(port_frame, text="Connect", command=lambda: connect_serial(port_variable.get()))
    connect_button.pack(side=tk.LEFT, padx=5, pady=5)
else:
    Label(port_frame, text="No serial ports found. Please connect your Arduino.").pack(padx=5, pady=5)

status_label = Label(app, text="Not Connected", fg="red")
status_label.pack(pady=5)

# --- Stepper Control Sliders ---
slider_frame = LabelFrame(app, text="Individual Stepper Motor Control")
slider_frame.pack(padx=10, pady=10, fill="x")

stepper_sliders = {} 

motors = ['X', 'Y', 'Z']
for motor_id in motors:
    label = Label(slider_frame, text=f"{motor_id} Motor")
    label.pack(pady=(10, 0))
    
    slider = Scale(
        slider_frame,
        from_=-slider_range,
        to=slider_range,
        orient=HORIZONTAL,
        length=400,
        resolution=10, # How many steps each 'tick' represents on the slider
        command=create_slider_handler(motor_id) # Assign motor-specific handler
    )
    slider.set(0) # Set slider to the center initially
    slider.pack(pady=5)
    stepper_sliders[motor_id] = slider

def reset_all_sliders():
    for motor_id, slider in stepper_sliders.items():
        slider.set(0) # Reset GUI slider to center
        previous_slider_values[motor_id] = 0 

reset_button = Button(app, text="Reset All Sliders", command=reset_all_sliders)
reset_button.pack(pady=10)

def on_closing():
    if ser and ser.is_open:
        ser.close()
        print("Serial port closed.")
    app.destroy()

app.protocol("WM_DELETE_WINDOW", on_closing)
app.mainloop()