#include <SoftwareSerial.h>
#include <Servo.h>
#include <U8g2lib.h>
#include <SPI.h>

// Motor pins for L298N
#define IN1 2
#define IN2 3
#define IN3 4
#define IN4 5
#define ENA 6
#define ENB 9

#define SERVO_PIN 12
#define LEFT_IR A0
#define RIGHT_IR A1

// Initialize OLED (for eye animation)
U8G2_SH1106_128X64_NONAME_1_4W_HW_SPI u8g2(U8G2_R0, 10, 7, 8); // CS, DC, RST

Servo headServo;
SoftwareSerial BTSerial(A4, A5);  // RX, TX

// Eye animation states
bool isAngry = false;
bool isBlinking = false;
int eyeOffsetX = 0, eyeOffsetY = 0;
int moveState = 0;
unsigned long lastBlink = 0;
unsigned long lastEyeMove = 0;
unsigned long lastIRCheck = 0;
unsigned long lastServoMove = 0;  // For random servo movement timing
int servoMoveDelay = 0;  // Delay for random servo movement
bool isAliveMode = false;  // Track alive mode state

void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600);
  
  // Motor control pins setup
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  
  // Set motor speed to full
  analogWrite(ENA, 255);
  analogWrite(ENB, 255);
  
  // Initialize servo
  headServo.attach(SERVO_PIN);
  headServo.write(90);  // Initial position
  
  // Initialize OLED for animation
  u8g2.begin();
  
  // Initialize IR sensors
  pinMode(LEFT_IR, INPUT);
  pinMode(RIGHT_IR, INPUT);
  
  // Wake-up effect (servo & eye blink)
  wakeUpEffect();
  
  Serial.println("BT Monitor Ready");
}

void loop() {
  if (BTSerial.available()) {
    String input = BTSerial.readStringUntil('\n');
    input.trim();  // Remove any extra whitespace
    Serial.print("Received: ");
    Serial.println(input);
    
    if (input.indexOf("dance") >= 0) {
      Serial.println("Dance command recognized!");
      danceRoutine();
    } else {
      Serial.println("Unknown command");
    }
  }

  // Blink every 2 seconds
  unsigned long now = millis();
  if (!isBlinking && now - lastBlink > 2000) {
    isBlinking = true;
    lastBlink = now;
  }
  if (isBlinking && now - lastBlink > 150) {
    isBlinking = false;
  }

  // Update eye direction (only if not angry)
  if (!isAngry) updateEyeDirection();

  // IR sensor check (Anger mode triggered by obstacle)
  if (now - lastIRCheck > 100) {
    lastIRCheck = now;
    int left = digitalRead(LEFT_IR);
    int right = digitalRead(RIGHT_IR);
    isAngry = (left == LOW || right == LOW);  // When IR detects something

    if (left == LOW) headServo.write(45);  // Move servo to the left
    else if (right == LOW) headServo.write(135);  // Move servo to the right
    else headServo.write(90);  // Keep servo at center when no detection
  }

  // Randomly move servo every 4-8 seconds in Alive Mode
  if (isAliveMode && now - lastServoMove >= servoMoveDelay) {
    // Set a new random delay for the next movement (between 4-8 seconds)
    servoMoveDelay = random(4000, 8000);
    lastServoMove = now;
    
    // Move the servo randomly between 45 (left) and 135 (right)
    int randomPosition = random(45, 135);
    slowServoMove(randomPosition);  // Move servo slowly
  }

  // Draw current eye frame on OLED
  drawEyes();
}

// Wake-up effect: servo movement & eye animation
void wakeUpEffect() {
  // Blink twice
  for (int i = 0; i < 2; i++) {
    isBlinking = true;
    delay(150);
    isBlinking = false;
    delay(150);
  }

  // Servo movement to show waking up
  headServo.write(45); // Look left
  delay(500);
  headServo.write(135); // Look right
  delay(500);
  headServo.write(90); // Centered

  // Randomly move head left and right for realism
  for (int i = 0; i < 5; i++) {
    headServo.write(random(45, 135));  // Random position between 45 and 135 degrees
    delay(300);
  }
  
  // Enable alive mode after wakeup
  isAliveMode = true;
}

// Slow servo movement (for realistic alive movement)
void slowServoMove(int targetPosition) {
  int currentPosition = headServo.read();
  int stepDelay = 15; // Reduced delay for smoother movement, you can adjust this for your preference

  // To prevent overshooting, ensure servo direction is correct
  if (currentPosition < targetPosition) {
    for (int pos = currentPosition; pos <= targetPosition; pos++) {
      headServo.write(pos);
      int randomDelay = random(10, 30);  // Random delay between 10ms to 30ms for each step
      delay(randomDelay);  // Add delay between each step for smooth movement
    }
  } else {
    for (int pos = currentPosition; pos >= targetPosition; pos--) {
      headServo.write(pos);
      int randomDelay = random(10, 30);  // Random delay between 10ms to 30ms for each step
      delay(randomDelay);  // Add delay between each step for smooth movement
    }
  }
}

// Dance routine: move motors and servo for 10 seconds
void danceRoutine() {
  unsigned long startTime = millis();

  while (millis() - startTime < 10000) {
    // Move Forward
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    headServo.write(45);  // Head left
    delay(500);

    // Move Backward
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    headServo.write(135); // Head right
    delay(500);
  }

  // Stop motors and reset head
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  headServo.write(90);  // Centered
}

// Draw current eye frame based on states
void drawEyes() {
  u8g2.firstPage();
  do {
    // If eyes are angry, show them differently
    if (isAngry) {
      u8g2.drawRBox(30 + eyeOffsetX, 26 + eyeOffsetY, 28, 14, 6);  // Angry left eye
      u8g2.drawRBox(70 + eyeOffsetX, 26 + eyeOffsetY, 28, 14, 6);  // Angry right eye
      u8g2.drawLine(30 + eyeOffsetX, 33 + eyeOffsetY, 58 + eyeOffsetX, 33 + eyeOffsetY); // Angry eyebrow
      u8g2.drawLine(70 + eyeOffsetX, 33 + eyeOffsetY, 98 + eyeOffsetX, 33 + eyeOffsetY); // Angry eyebrow
    } 
    else if (isBlinking) {
      u8g2.drawRBox(30, 42, 28, 4, 2); // Left eye closed line
      u8g2.drawRBox(70, 42, 28, 4, 2); // Right eye closed line
    } 
    else {
      // Draw normal eyes
      u8g2.drawRBox(30 + eyeOffsetX, 26 + eyeOffsetY, 28, 28, 6);  // Left eye
      u8g2.drawRBox(70 + eyeOffsetX, 26 + eyeOffsetY, 28, 28, 6);  // Right eye
    }
  } while (u8g2.nextPage());
}

// Update eye direction randomly
void updateEyeDirection() {
  unsigned long now = millis();
  if (now - lastEyeMove > 3000) {
    lastEyeMove = now;
    moveState = (moveState + 1) % 5;
    switch (moveState) {
      case 0: eyeOffsetX = 0; eyeOffsetY = 0; break;
      case 1: eyeOffsetX = 4; eyeOffsetY = 0; break;
      case 2: eyeOffsetX = -4; eyeOffsetY = 0; break;
      case 3: eyeOffsetX = 0; eyeOffsetY = -3; break;
      case 4: eyeOffsetX = 0; eyeOffsetY = 3; break;
    }
  }
}
