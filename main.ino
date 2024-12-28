#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define IR_PIN 2            // IR sensor pin
#define BUZZER_PIN 3        // Buzzer pin
#define LED_RED_PIN 4       // Red pin of HW487 LED
#define LED_GREEN_PIN 5     // Green pin of HW487 LED
#define LED_BLUE_PIN 6      // Blue pin of HW487 LED
#define RST_PIN 9           // RFID reset pin
#define SS_PIN 10           // RFID SDA/SS pin

const String authorizedUID = "A3 37 21 30"; // Authorized RFID tag UID
bool authorized = false;                    // Tracks if access is authorized
bool alertActive = false;                   // Tracks if intruder alert is active
bool buzzing = false;                       // Tracks if the buzzer is active
unsigned long accessEndTime = 0;            // Tracks end time of authorized access
unsigned long lastBuzzTime = 0;             // Tracks time for cop car siren pattern
unsigned long lastLEDFlashTime = 0;         // Tracks time for blue LED flashing
MFRC522 rfid(SS_PIN, RST_PIN);

Servo doorServo;                            // Servo motor for door

void setup() {
    Serial.begin(9600);                      // Initialize serial communication
    SPI.begin();
    rfid.PCD_Init();
    
    pinMode(IR_PIN, INPUT);
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(LED_RED_PIN, OUTPUT);
    pinMode(LED_GREEN_PIN, OUTPUT);
    pinMode(LED_BLUE_PIN, OUTPUT);

    doorServo.attach(7);                    // Connect the servo to Pin 7
    doorServo.write(0);                     // Start with the door closed (0 degrees)

    Serial.println("System Initialized");
}

void loop() {
    // Check for authorized RFID tag
    if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
        String uid = "";
        for (byte i = 0; i < rfid.uid.size; i++) {
            uid += String(rfid.uid.uidByte[i] < 0x10 ? "0" : "");
            uid += String(rfid.uid.uidByte[i], HEX);
            if (i != rfid.uid.size - 1) uid += " ";
        }
        uid.toUpperCase();

        if (uid == authorizedUID) {
            Serial.println("Access Granted");

            if (alertActive) {
                alertActive = false;
                buzzing = false;
                noTone(BUZZER_PIN);          // Stop the buzzer
                turnOffLEDs();               // Stop the blue LED flashing
            } else if (!authorized) {
                authorized = true;
                accessEndTime = millis() + 5000;  // Set access end time to 5 seconds from now
                turnOnGreenLED();
                slowMoveServo(90);           // Slowly open the door
            }
        } else {
            Serial.println("Access Denied - Unauthorized RFID Tag");

            if (authorized) {
                slowMoveServo(0);            // Slowly close the door
                authorized = false;
                buzzing = true;
                alertActive = true;
                Serial.println("Intruder detected!");  // Send alert to Python script
            } else {
                buzzing = true;
                alertActive = true;
                Serial.println("Intruder detected!");  // Send alert to Python script
            }
        }
        rfid.PICC_HaltA();                   // Halt the RFID reader
    }

    // Check if authorized access time has expired
    if (authorized && millis() > accessEndTime) {
        authorized = false;
        turnOffLEDs();
        slowMoveServo(0);                    // Slowly close the door
    }

    // IR Sensor detection logic for unauthorized entry
    if (digitalRead(IR_PIN) == LOW && !authorized) {
        Serial.println("Unauthorized Entry Detected!"); // Send intruder alert to Python script
        buzzing = true;
        alertActive = true;
        Serial.println("Intruder detected!");  // Send alert to Python script
    } else if (!authorized) {
        turnOffLEDs();
    }

    // Handle continuous buzzer sound in "cop car" style and blue LED flashing if buzzing is active
    if (buzzing) {
        unsigned long currentTime = millis();

        // Buzzer "cop car" siren pattern
        if (currentTime - lastBuzzTime >= 500) {  // Switch sound every 500ms
            lastBuzzTime = currentTime;
            int frequency = (currentTime / 500 % 2 == 0) ? 1000 : 700; // Alternate frequency
            tone(BUZZER_PIN, frequency);
        }

        // Blue LED flashing pattern
        if (currentTime - lastLEDFlashTime >= 250) {  // Toggle LED every 250ms
            lastLEDFlashTime = currentTime;
            digitalWrite(LED_BLUE_PIN, !digitalRead(LED_BLUE_PIN));  // Toggle the blue LED
        }
    } else {
        noTone(BUZZER_PIN);               // Turn off buzzer when not buzzing
        digitalWrite(LED_BLUE_PIN, LOW);  // Ensure blue LED is off when not in alert state
    }
}

// Slow down servo movement
void slowMoveServo(int targetAngle) {
    int currentAngle = doorServo.read();  // Get the current angle of the servo
    int step = (targetAngle > currentAngle) ? 1 : -1;  // Determine the direction of movement
    
    // Gradually move to the target angle
    while (currentAngle != targetAngle) {
        currentAngle += step;                   // Move one step
        doorServo.write(currentAngle);           // Write the current angle to the servo
        delay(15);                               // Adjust delay to control speed (increase for slower)
    }
}

// Turn on the green LED for authorized access
void turnOnGreenLED() {
    digitalWrite(LED_GREEN_PIN, HIGH);
    digitalWrite(LED_RED_PIN, LOW);
    digitalWrite(LED_BLUE_PIN, LOW);
}

// Turn off all LEDs
void turnOffLEDs() {
    digitalWrite(LED_GREEN_PIN, LOW);
    digitalWrite(LED_RED_PIN, LOW);
    digitalWrite(LED_BLUE_PIN, LOW);
}
