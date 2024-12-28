# Security Lighting System

The Security Lighting System is an intelligent and automated setup designed to enhance security using RFID access control, IR sensors for unauthorized entry detection, and an alert mechanism with LEDs and a buzzer. The system integrates servo motor control for door access and employs various patterns for visual and audio alerts.

## Features
- **RFID Access Control**: Grants or denies access based on authorized RFID tags.
- **Intruder Detection**: Utilizes an IR sensor to detect unauthorized entries.
- **Alert Mechanisms**:
  - **Buzzer**: Emits a "cop car" siren pattern during alerts.
  - **LED Indicators**:
    - Green for authorized access.
    - Red and blue for unauthorized alerts, with flashing blue for active alarms.
- **Servo-Controlled Door**: Gradually opens/closes the door based on access status.

## Components Used
- **Microcontroller**: Arduino
- **RFID Module**: MFRC522 for tag reading
- **IR Sensor**: Detects motion and unauthorized entries
- **LEDs**: RGB LEDs for visual indicators
- **Servo Motor**: Controls door opening/closing
- **Buzzer**: Produces siren alerts

## Key Functionalities
1. **Access Authorization**:
   - Compares scanned RFID tag UID with an authorized UID.
   - Opens the door and lights the green LED for authorized users.
2. **Intruder Alerts**:
   - Activates buzzer and flashing blue LED for unauthorized access or IR detection.
   - Sends intrusion alerts to the connected system via Serial communication.
3. **Timed Access**:
   - Automatically revokes access after a set duration (e.g., 5 seconds).
4. **Smooth Door Movement**:
   - Gradual opening/closing of the servo-controlled door for smooth operation.

## How to Use
1. **Setup**:
   - Connect the components as specified in the source code.
   - Upload the code to the Arduino using the Arduino IDE.
2. **Operation**:
   - Scan an RFID tag for access control.
   - Observe LED indicators and buzzer alerts for access status.
   - Monitor the door's operation as it opens/closes for authorized users.

## Future Enhancements
- Integration with a mobile app for remote monitoring and control.
- Logging access attempts and intrusion alerts for security auditing.
- Adding a camera module for visual verification.

## Requirements
- **Arduino IDE** for code compilation and upload.
- Required libraries:
  - `MFRC522` for RFID functionality.
  - `Servo` for servo motor control.

## Code Highlights
The system uses modular functions to ensure efficient operation:
- `slowMoveServo()`: Gradual servo movements.
- `turnOnGreenLED()`: Handles visual indicators for access.
- `turnOffLEDs()`: Resets all LED indicators.
- Serial communication for external system alerts.

---

### Project Maintainer
Developed by Shawon Ghosh. Contributions and suggestions are welcome!
