#include <Wire.h>
#include <DHT.h>
#include <math.h>

#define R 3
#define G 5
#define B 11

#define irPin 7
#define buzzer 8

#define trigPin 9
#define echoPin 10

#define DHTPIN 6
#define DHTTYPE DHT11

const int MPU = 0x68;

DHT dht(DHTPIN, DHTTYPE);

int16_t AcX, AcY, AcZ;
unsigned long lastFallTime = 0;

void setColor(int r, int g, int b) {
  digitalWrite(R, r);
  digitalWrite(G, g);
  digitalWrite(B, b);
}

int getDistance() {
  long duration;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH, 30000);
  if (duration == 0) return -1;

  return duration * 0.034 / 2;
}

void readMPU() {
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true);

  AcX = Wire.read() << 8 | Wire.read();
  AcY = Wire.read() << 8 | Wire.read();
  AcZ = Wire.read() << 8 | Wire.read();
}

void setup() {
  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);

  pinMode(irPin, INPUT);
  pinMode(buzzer, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.begin(9600);
  dht.begin();

  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
}

void loop() {

  int irState = digitalRead(irPin);
  int distance = getDistance();
  float temp = dht.readTemperature();

  readMPU();

  float ax = AcX;
  float ay = AcY;
  float az = AcZ;
  float A = sqrt(ax*ax + ay*ay + az*az);

  Serial.print("A: "); Serial.print(A);
  Serial.print(" | D: "); Serial.print(distance);
  Serial.print(" | T: "); Serial.print(temp);
  Serial.print(" | IR: "); Serial.println(irState);

  // -------- FALL (LED ONLY) --------
  if (millis() - lastFallTime > 4000) {

    if (A < 8000) {

      delay(200);
      readMPU();

      float ax2 = AcX;
      float ay2 = AcY;
      float az2 = AcZ;
      float A2 = sqrt(ax2*ax2 + ay2*ay2 + az2*az2);

      if (A2 > 22000) {

        lastFallTime = millis();

        for (int i = 0; i < 6; i++) {
          setColor(1,0,0);   // RED
          delay(180);
          setColor(1,1,0);   // YELLOW
          delay(180);
        }

        return;
      }
    }
  }

  // -------- STATES --------
  int IR = (irState == HIGH) ? 1 : 0;
  int DHTv = (!isnan(temp) && temp > 30) ? 1 : 0;

  int ULTRA = 0;
  int stage = 0;

  if (distance != -1) {
    if (distance < 20) { ULTRA = 1; stage = 3; }
    else if (distance < 50) { ULTRA = 1; stage = 2; }
    else if (distance < 100) { ULTRA = 1; stage = 1; }
  }

  // -------- RGB (all sensors) --------
  setColor(IR, ULTRA, DHTv);

  // -------- BUZZER (everything except MPU) --------

  // IR alert tone
  if (IR == 1) {
    tone(buzzer, 1800);
    delay(150);
    noTone(buzzer);
    delay(150);
    return;
  }

  // DHT alert tone
  if (DHTv == 1) {
    tone(buzzer, 1000);
    delay(300);
    noTone(buzzer);
    delay(300);
    return;
  }

  // Ultrasonic tones
  if (stage == 3) {
    tone(buzzer, 2200);
  }
  else if (stage == 2) {
    tone(buzzer, 1400);
    delay(150);
    noTone(buzzer);
    delay(150);
  }
  else if (stage == 1) {
    tone(buzzer, 800);
    delay(400);
    noTone(buzzer);
    delay(400);
  }
  else {
    noTone(buzzer);
  }

  delay(50);
}
