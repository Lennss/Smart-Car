#include <BluetoothSerial.h>

BluetoothSerial bt;

// Esp32 Devkit v1
// int enA = 12, inA1 = 14, inA2 = 27, speedA = 255;
// int enB = 33, inB1 = 26, inB2 = 25, speedB = 150;

// Esp32 
int enA = 13, inA1 = 12, inA2 = 14, speedA = 255;
int enB = 25, inB1 = 27, inB2 = 26, speedB = 150;

void setup() {
  Serial.begin(9600);
  bt.begin("Pajero Sport"); // Nama Bluetooth

  // Setup pin motor
  pinMode(enA, OUTPUT);
  pinMode(inA1, OUTPUT);
  pinMode(inA2, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(inB1, OUTPUT);
  pinMode(inB2, OUTPUT);
}

void loop() {
  if (bt.available() > 0) {
    char data = bt.read();
    Serial.println(data);
    car(data);
  }
}

void car(char data) {
  switch (data) {
    case 'F': // Maju
      digitalWrite(inA1, HIGH);
      digitalWrite(inA2, LOW);
      analogWrite(enA, speedA);
      digitalWrite(inB1, LOW);
      digitalWrite(inB2, HIGH);
      analogWrite(enB, speedB);
      break;

    case 'B': // Mundur
      digitalWrite(inA1, LOW);
      digitalWrite(inA2, HIGH);
      analogWrite(enA, speedA);
      digitalWrite(inB1, HIGH);
      digitalWrite(inB2, LOW);
      analogWrite(enB, speedB);
      break;

    case 'S': // Stop
      analogWrite(enA, 0);
      analogWrite(enB, 0);
      break;

    case 'R': // Kanan
      digitalWrite(inA1, LOW);
      digitalWrite(inA2, HIGH);
      analogWrite(enA, speedA);
      digitalWrite(inB1, LOW);
      digitalWrite(inB2, HIGH);
      analogWrite(enB, speedB);
      break;

    case 'L': // Kiri
      digitalWrite(inA1, HIGH);
      digitalWrite(inA2, LOW);
      analogWrite(enA, speedA);
      digitalWrite(inB1, HIGH);
      digitalWrite(inB2, LOW);
      analogWrite(enB, speedB);
      break;


    case 'G': // Maju Kanan
      digitalWrite(inA1, HIGH);
      digitalWrite(inA2, LOW);
      analogWrite(enA, speedA);
      digitalWrite(inB1, LOW);
      digitalWrite(inB2, LOW);
      analogWrite(enB, speedB / 2);
      break;
      
    case 'I': // Maju Kiri
      digitalWrite(inA1, LOW);
      digitalWrite(inA2, LOW);
      analogWrite(enA, speedA / 2);
      digitalWrite(inB1, LOW);
      digitalWrite(inB2, HIGH);
      analogWrite(enB, speedB);
      break;

    case 'H': // Mundur Kanan
      digitalWrite(inA1, LOW);
      digitalWrite(inA2, HIGH);
      analogWrite(enA, speedA);
      digitalWrite(inB1, LOW);
      digitalWrite(inB2, LOW);
      analogWrite(enB, speedB / 2);
      break;

    case 'J': // Mundur Kiri
      digitalWrite(inA1, LOW);
      digitalWrite(inA2, LOW);
      analogWrite(enA, speedA / 2);
      digitalWrite(inB1, HIGH);
      digitalWrite(inB2, LOW);
      analogWrite(enB, speedB);
      break;


    case '0': speedA = 0; speedB = 0; break;
    case '1': speedA = speedB = 30; break;
    case '2': speedA = speedB = 60; break;
    case '3': speedA = speedB = 90; break;
    case '4': speedA = speedB = 120; break;
    case '5': speedA = speedB = 150; break;
    case '6': speedA = speedB = 180; break;
    case '7': speedA = speedB = 200; break;
    case '8': speedA = speedB = 220; break;
    case '9': speedA = speedB = 240; break;
    case 'q': speedA = speedB = 255; break;

    case 'D':
      speedA = 0;
      speedB = 0;
      analogWrite(enA, 0);
      analogWrite(enB, 0);
      break;

    default:
      break;
  }
}
