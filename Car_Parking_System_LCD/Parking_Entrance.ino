#include <LiquidCrystal_I2C.h>
#include <SPI.h>             // RFID library
#include <MFRC522.h>
#include <Wire.h>
#include <Servo.h>           // Servo motor library
#include <HCSR04.h>          // Ultrasonic sensor library

#define POSITIVE 0

// Ultrasonic sensor pins
#define TRIG_PIN 5
#define ECHO_PIN 6
#define TRIG_PIN2 2
#define ECHO_PIN2 3

// RFID pins
#define SS_PIN 10
#define RST_PIN 9

// Timing constants
const unsigned long SERVO_OPEN_TIME = 5000;
const unsigned long LCD_REFRESH_TIME = 1000;
const unsigned long DISPLAY_MESSAGE_TIME = 3000;
const unsigned long BUZZER_BEEP_TIME = 1000;

// Global variables
unsigned long lastServo1Time = 0;
unsigned long lastServo2Time = 0;
unsigned long lastLcdRefresh = 0;
unsigned long messageStartTime = 0;
unsigned long buzzerStartTime = 0;

// State flags
boolean RFIDMode = false;
boolean servo1Opening = false;
boolean servo2Opening = false;
boolean buzzerActive = false;

String currentMessage = "";
int currentMessageLine = 0;
String tagUID = "23 CA C5 26"; // Authorized tag UID

MFRC522 mfrc522(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7);
UltraSonicDistanceSensor distanceSensor(TRIG_PIN, ECHO_PIN);
UltraSonicDistanceSensor distanceSensor2(TRIG_PIN2, ECHO_PIN2);

Servo myservo;
Servo myservo2;
int buzzer = 8;

void setup() {
  lcd.setBacklightPin(3, POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.begin(16, 2);

  SPI.begin();
  mfrc522.PCD_Init();

  Serial.begin(9600);

  myservo.attach(4);
  myservo2.attach(7);

  pinMode(buzzer, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(TRIG_PIN2, OUTPUT);
  pinMode(ECHO_PIN2, INPUT);

  lcd.setCursor(1, 0);
  lcd.print("Car parking");
  lcd.setCursor(1, 1);
  lcd.print(" System ");
  delay(2000);
  lcd.clear();

  myservo.write(0);
  myservo2.write(0);
}

void displayTemporaryMessage(String line1, String line2) {
  currentMessage = line1;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(line1);
  lcd.setCursor(0, 1);
  lcd.print(line2);
}

String getCardUID() {
  String content = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  return content.substring(1);
}

void handleRFID() {
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  String content = getCardUID();
  Serial.print("Card UID: ");
  Serial.println(content);

  if (content == tagUID) {
    displayTemporaryMessage("Tag Matched", "");
    servo1Opening = true;
    lastServo1Time = millis();
    RFIDMode = false;
  } else {
    displayTemporaryMessage("Wrong Tag Shown", "Access Denied");
    buzzerActive = true;
    buzzerStartTime = millis();
  }
}

void handleServos() {
  unsigned long currentTime = millis();

  // Servo 1 (RFID)
  if (servo1Opening && (currentTime - lastServo1Time >= SERVO_OPEN_TIME)) {
    myservo.write(0);
    servo1Opening = false;
    RFIDMode = false;
    displayTemporaryMessage("Door Closed", "");
    delay(3000);
    currentMessage = "";
  } else if (servo1Opening) {
    myservo.write(90);
  }

  // Servo 2 (Exit)
  if (servo2Opening && (currentTime - lastServo2Time >= SERVO_OPEN_TIME)) {
    myservo2.write(0);
    servo2Opening = false;
  } else if (servo2Opening) {
    myservo2.write(90);
  }
}

void handleBuzzer() {
  if (buzzerActive) {
    if (millis() - buzzerStartTime < BUZZER_BEEP_TIME) {
      tone(buzzer, BUZZER_BEEP_TIME);
    } else if (millis() - buzzerStartTime < BUZZER_BEEP_TIME * 2) {
      noTone(buzzer);
    } else {
      buzzerActive = false;
      digitalWrite(buzzer, LOW);
      currentMessage = "";
    }
  }
}

void handleDisplay() {
  if (millis() - lastLcdRefresh >= LCD_REFRESH_TIME) {
    if (currentMessage == "") {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(" Bienvenido ");
      lcd.setCursor(0, 1);
      lcd.print(" Get close...");
    }
    lastLcdRefresh = millis();
  }
}

void loop() {
  float distance = distanceSensor.measureDistanceCm();
  float distance2 = distanceSensor2.measureDistanceCm();

  Serial.println(distance);

  // Entry detection
  if (!RFIDMode) {
    if (distance < 5 && distance >= 0) {
      RFIDMode = true;
      if (RFIDMode) {
        displayTemporaryMessage("RFID Door Lock", "Show Your Tag");
        handleRFID();
      }
    }
  } else {
    handleRFID();
  }

  // Exit detection
  if (distance2 < 5 && distance2 >= 0 && !servo2Opening) {
    servo2Opening = true;
    lastServo2Time = millis();
  }

  // Handle system tasks
  handleServos();
  handleBuzzer();
  handleDisplay();

  delay(10); // Avoid CPU overload
}
