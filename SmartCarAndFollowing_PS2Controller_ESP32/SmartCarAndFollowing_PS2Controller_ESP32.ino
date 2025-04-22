#include <PS2X_lib.h>      

// Motor Pin 
#define leftm1     27   // inB1 (Left Motor)
#define leftm2     26   // inB2 (Left Motor)
#define rightm1    12   // inA1 (Right Motor)
#define rightm2    14   // inA2 (Right Motor)
#define left_EN    25   // enB (Left Motor Enable)
#define right_EN   13   // enA (Right Motor Enable)

// PS2 Controller Pin 
#define PS2_DAT    15 
#define PS2_CMD    2  
#define PS2_SEL    4  
#define PS2_CLK    16  

// Ultrasonic Sensor Pins
#define TRIG_PIN   5   
#define ECHO_PIN   18 

// Following
#define MAX_MOTOR_SPEED 255
#define FOLLOW_DISTANCE 20 // Jarak mobil mengikuti(CM)
#define MIN_DISTANCE    10 // Jarak minimal mobil berhenti mengikuti(CM)

PS2X ps2x;
int error = 0;
byte type = 0;
bool followMode = false;

void setup() {
  Serial.begin(9600);

  pinMode(leftm1, OUTPUT);
  pinMode(leftm2, OUTPUT);
  pinMode(rightm1, OUTPUT);
  pinMode(rightm2, OUTPUT);
  pinMode(left_EN, OUTPUT);
  pinMode(right_EN, OUTPUT);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  digitalWrite(left_EN, LOW);
  digitalWrite(right_EN, LOW);

  error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, true, true);
  if (error == 0) {
    Serial.println("Found Controller, configured successfully");
  } else {
    Serial.print("Error: ");
    Serial.println(error);
  }

  type = ps2x.readType();
  switch (type) {
    case 0: Serial.println("Unknown Controller type found"); break;
    case 1: Serial.println("DualShock Controller found"); break;
    case 2: Serial.println("GuitarHero Controller found"); break;
    case 3: Serial.println("Wireless Sony DualShock Controller found"); break;
  }
}

void loop() {
  ps2x.read_gamepad(false, 0); 

  if (ps2x.ButtonPressed(PSB_SELECT)) {
    followMode = !followMode;
    if (followMode) {
      Serial.println("Switching to Following Mode");
    } else {
      Serial.println("Switching to Manual Mode");
      stop();
    }
    delay(200);
  }

  if (followMode) {
    followObject();
  } else {
    int rightY = ps2x.Analog(PSS_RY); 
    int leftX = ps2x.Analog(PSS_LX);  
    int btnR2 = ps2x.Button(PSB_R2);
    int btnL2 = ps2x.Button(PSB_L2);
    int btnR = ps2x.Button(PSB_PAD_RIGHT);
    int btnL = ps2x.Button(PSB_PAD_LEFT);

    if (rightY > 200) {
      backward();
    } else if (rightY < 100) {
      forward();
    } else if (leftX < 100) {
      left2();
    } else if (leftX > 200) {
      right2();
    } else if (btnR2) {
      forward();
    } else if (btnL2) {
      backward();
    } else {
      stop();
    }

    if (btnR) {
      right();
    } else if (btnL) {
      left();
    }
  }

  delay(50);
}

float getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  long duration = pulseIn(ECHO_PIN, HIGH);
  float distance = duration * 0.034 / 2;
  return distance;
}

void followObject() {
  float distance = getDistance();
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance < MIN_DISTANCE) {
    stop();
  } else if (distance < FOLLOW_DISTANCE - 5) {
    backward();
  } else if (distance > FOLLOW_DISTANCE + 5) {
    forward();
  } else {
    stop();
  }
}

void right2() {
  Serial.println("Turning sharply right");
  digitalWrite(left_EN, HIGH);
  digitalWrite(right_EN, HIGH);
  digitalWrite(leftm1, LOW);
  digitalWrite(leftm2, HIGH);
  digitalWrite(rightm1, LOW);
  digitalWrite(rightm2, HIGH);
}

void left2() {
  Serial.println("Turning sharply left");
  digitalWrite(left_EN, HIGH);
  digitalWrite(right_EN, HIGH);
  digitalWrite(leftm1, HIGH);
  digitalWrite(leftm2, LOW);
  digitalWrite(rightm1, HIGH);
  digitalWrite(rightm2, LOW);
}

void backward() {
  Serial.println("Moving backward");
  digitalWrite(left_EN, HIGH);
  digitalWrite(right_EN, HIGH);
  digitalWrite(leftm1, HIGH);
  digitalWrite(leftm2, LOW);
  digitalWrite(rightm1, LOW);
  digitalWrite(rightm2, HIGH);
}

void forward() {
  Serial.println("Moving forward");
  digitalWrite(left_EN, HIGH);
  digitalWrite(right_EN, HIGH);
  digitalWrite(leftm1, LOW);
  digitalWrite(leftm2, HIGH);
  digitalWrite(rightm1, HIGH);
  digitalWrite(rightm2, LOW);
}

void stop() {
  Serial.println("Stopping");
  digitalWrite(left_EN, LOW);
  digitalWrite(right_EN, LOW);
  digitalWrite(leftm1, LOW);
  digitalWrite(leftm2, LOW);
  digitalWrite(rightm1, LOW);
  digitalWrite(rightm2, LOW);
}

void right() {
  Serial.println("Turning right");
  digitalWrite(left_EN, HIGH);
  digitalWrite(right_EN, LOW);
  digitalWrite(leftm1, LOW);
  digitalWrite(leftm2, HIGH);
  digitalWrite(rightm1, LOW);
  digitalWrite(rightm2, LOW);
}

void left() {
  Serial.println("Turning left");
  digitalWrite(left_EN, LOW);
  digitalWrite(right_EN, HIGH);
  digitalWrite(leftm1, LOW);
  digitalWrite(leftm2, LOW);
  digitalWrite(rightm1, HIGH);
  digitalWrite(rightm2, LOW);
}
