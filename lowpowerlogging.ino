// Libraries
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <RTClib.h>  // for DS3231 RTC
#include <LowPower.h>  // for sleep

// Pin Definitions
const int SD_CS_PIN = 10;    // Chip Select pin for SD card
const int SD_PWR_PIN = 8;    // GPIO to control SD card power (MOSFET)

RTC_DS3231 rtc;  // Real Time Clock object

// Variables
unsigned long wakeCount = 0;  // Counter for wake-up cycles

void setup() {
  // Initialize pins
  pinMode(SD_CS_PIN, OUTPUT);
  digitalWrite(SD_CS_PIN, HIGH);  // Deselect SD card
  pinMode(SD_PWR_PIN, OUTPUT);
  digitalWrite(SD_PWR_PIN, HIGH); // SD card power off (MOSFET OFF)

  Wire.begin();
  rtc.begin();

  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  randomSeed(analogRead(A0)); // Random seed setup
}

void loop() {
  wakeCount++;

  // 1. Power on SD Card
  digitalWrite(SD_PWR_PIN, LOW);
  delay(50);  // Wait for SD card to stabilize

  // 2. Initialize SD Card
  if (!SD.begin(SD_CS_PIN)) {
    powerDownSD();
    sleepForTenMinutes();
    return;
  }

  // 3. Get Current Time for Filename
  DateTime now = rtc.now();
  char filename[20];
  sprintf(filename, "%02d%02d%02d_%02d%02d.CSV",
          now.year() % 100, now.month(), now.day(), now.hour(), now.minute());

  // 4. Create and Write to File
  File dataFile = SD.open(filename, FILE_WRITE);
  if (dataFile) {
    for (int i = 0; i < 5; i++) {
      unsigned long timestamp = now.unixtime() + (i * 10);  // Dummy variation
      int sensorValue = random(0, 1000);

      dataFile.print(timestamp);
      dataFile.print(",");
      dataFile.println(sensorValue);

      delay(200);
    }
    dataFile.close();
  }

  // 5. Power Down SD Card
  powerDownSD();

  // 6. Sleep for 10 minutes
  sleepForTenMinutes();
}

void powerDownSD() {
  digitalWrite(SD_CS_PIN, HIGH);  // Deselect SD
  pinMode(SD_CS_PIN, INPUT);
  pinMode(MOSI, INPUT);
  pinMode(MISO, INPUT);
  pinMode(SCK, INPUT);
  digitalWrite(SD_PWR_PIN, HIGH); // Turn off SD card power
}

void sleepForTenMinutes() {
  for (int i = 0; i < 75; i++) {  // 75 x 8s = 600s (10 minutes)
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  }
}
