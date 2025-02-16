import serial
import cv2
import time

# Set up the serial connection (replace 'COM3' with your Arduino's port)
arduino = serial.Serial('COM5', 9600)  # On Windows, it could be 'COM3', on Mac/Linux it could be '/dev/ttyUSB0'
time.sleep(2)  # Give some time for the serial connection to establish

# Open the camera (0 is the default camera)
cap = cv2.VideoCapture(0)

# Check if the camera opened successfully
if not cap.isOpened():
    print("Error: Could not open camera.")
    exit()

while True:
    # Read data from Arduino serial
    if arduino.in_waiting > 0:
        message = arduino.readline().decode('utf-8').strip()  # Read the incoming data
        print(f"Message from Arduino: {message}")
        
        # If an intruder is detected, capture a photo
        if message == "Intruder detected!":
            print("Intruder alert! Taking photo...")
            
            # Capture a frame from the camera
            ret, frame = cap.read()
            if ret:
                # Save the captured image with a timestamp
                timestamp = time.strftime("%Y%m%d_%H%M%S")
                filename = f"intruder_{timestamp}.jpg"
                cv2.imwrite(filename, frame)
                print(f"Photo saved as {filename}")
            else:
                print("Failed to capture image.")
    
    # Exit the loop if the 'q' key is pressed
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Release the camera and close all OpenCV windows
cap.release()
cv2.destroyAllWindows()
