#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <math.h>

// ---------------- PIN DEFINITIONS ----------------

#define TRIG_PIN 2
#define ECHO_PIN 3
#define SOUND_PIN A0

// ---------------- LCD ----------------

LiquidCrystal_I2C lcd(0x27, 16, 2);

// ---------------- MPU6050 ----------------

Adafruit_MPU6050 mpu;

// ---------------- THRESHOLDS ----------------
// Change these after observing your actual readings.

#define SOUND_WARNING 600
#define SOUND_ALERT   750

#define VIB_WARNING   10.0
#define VIB_ALERT     12.0

// ---------------- SETUP ----------------

void setup() {

  Serial.begin(9600);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(SOUND_PIN, INPUT);

  Wire.begin();

  // LCD
  lcd.init();
  lcd.backlight();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SENTINEL ROVER");
  lcd.setCursor(0, 1);
  lcd.print("Starting...");

  delay(2000);

  // MPU6050
  if (!mpu.begin()) {

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("MPU6050 ERROR");
    lcd.setCursor(0, 1);
    lcd.print("Check Wiring");

    Serial.println("MPU6050 not detected!");

    while (1);
  }

  // MPU6050 configuration
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SENSORS READY");
  lcd.setCursor(0, 1);
  lcd.print("INSPECTION ON");

  delay(2000);
}

// ---------------- DISTANCE FUNCTION ----------------

float getDistance() {

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);

  if (duration == 0) {
    return -1;
  }

  float distance = duration * 0.0343 / 2.0;

  return distance;
}

// ---------------- MAIN LOOP ----------------

void loop() {

  // Get sensor values
  float distance = getDistance();

  int soundValue = analogRead(SOUND_PIN);

  sensors_event_t acceleration;
  sensors_event_t gyro;
  sensors_event_t temperature;

  mpu.getEvent(&acceleration, &gyro, &temperature);

  // Calculate total acceleration
  float ax = acceleration.acceleration.x;
  float ay = acceleration.acceleration.y;
  float az = acceleration.acceleration.z;

  float totalAcceleration =
      sqrt((ax * ax) +
           (ay * ay) +
           (az * az));

  // ---------------- SERIAL OUTPUT ----------------

  Serial.print("Distance: ");
  Serial.print(distance);

  Serial.print(" cm | Sound: ");
  Serial.print(soundValue);

  Serial.print(" | Vibration: ");
  Serial.print(totalAcceleration);

  Serial.print(" m/s2 | Temp: ");
  Serial.println(temperature.temperature);

  // ---------------- MACHINE HEALTH ----------------

  bool vibrationAlert = totalAcceleration > VIB_ALERT;
  bool vibrationWarning = totalAcceleration > VIB_WARNING;

  bool soundAlert = soundValue > SOUND_ALERT;
  bool soundWarning = soundValue > SOUND_WARNING;

  // ---------------- DISPLAY SENSOR DATA ----------------

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Vib:");
  lcd.print(totalAcceleration, 1);

  lcd.setCursor(0, 1);
  lcd.print("Sound:");
  lcd.print(soundValue);

  delay(2000);

  // ---------------- DISPLAY DISTANCE ----------------

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("DIST:");

  if (distance < 0) {
    lcd.print("ERROR");
  }
  else {
    lcd.print(distance, 1);
    lcd.print("cm");
  }

  lcd.setCursor(0, 1);

  if (distance > 0 && distance < 20) {
    lcd.print("OBSTACLE CLOSE");
  }
  else {
    lcd.print("PATH CLEAR");
  }

  delay(1500);

  // ---------------- HEALTH RESULT ----------------

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("MACHINE HEALTH");

  lcd.setCursor(0, 1);

  if (vibrationAlert || soundAlert) {

    lcd.print("CRITICAL");

  }
  else if (vibrationWarning || soundWarning) {

    lcd.print("WARNING");

  }
  else {

    lcd.print("NORMAL");
  }

  delay(2500);

  // ---------------- REINSPECTION ----------------

  if (vibrationAlert || soundAlert) {

    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("ABNORMAL DATA");

    lcd.setCursor(0, 1);
    lcd.print("REINSPECT");

    delay(2000);
  }
}