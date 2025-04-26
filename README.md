# 📦 Low Power Data Logger with Arduino, SD Card, and RTC

## Overview
This project implements a **battery-powered, ultra low-power data logger** using an **Arduino Pro Mini (3.3V 8MHz)**, a **microSD card**, and a **DS3231 RTC module**.  
It wakes up **every 10 minutes**, powers on the SD card, creates a **uniquely timestamped CSV file**, writes **5 rows of data**, then powers everything off and goes back to **deep sleep** to conserve battery.

---

## 🛠 Hardware Components
- Arduino Pro Mini 3.3V 8MHz (or similar low-power microcontroller)
- microSD Card Module (compatible with 3.3V logic)
- P-Channel MOSFET (for SD card power control)
- DS3231 Real Time Clock (RTC) Module
- 100kΩ pull-up resistor (MOSFET gate)
- 0.1μF decoupling capacitor (optional but recommended)
- Battery (3×AA NiMH or Li-ion regulated to 3.3V)
- Common components: jumper wires, breadboard or PCB

---

## 👋 Circuit Architecture
**Architecture Summary:**
```
[ Battery +3.3V ]
      |
     [ P-Channel MOSFET ]
        - Source (S) → Battery +3.3V
        - Drain (D)  → SD Card VCC
        - Gate (G)   → GPIO (D8) from MCU + 100kΩ pull-up to 3.3V
      |
[ Microcontroller (Arduino Pro Mini 3.3V 8MHz) ]
      |
      ├── D8  → MOSFET Gate control
      ├── D10 → SD card CS (Chip Select)
      ├── D11 → SD card MOSI
      ├── D12 → SD card MISO
      └── D13 → SD card SCK
[ SD Card Module ]
      |
    (VCC, GND, CS, MOSI, MISO, SCK)
[ RTC DS3231 Module ]
      ├── SDA → A4
      └── SCL → A5



                  VCC (3.3V)
                     |
                     |
                    100kΩ
                     |
                     |
GPIO Pin -------|Gate
                |    P-Channel MOSFET
                |Source    |Drain
                     |     |
                     |     |
                     |    100kΩ
                     |     |
                     |     |
                     ------+------ To SD Card VCC
                           |
                           |
                           |
                           |
                          GND

```

---
## 🤠 Design Highlights
- **Low Power Consumption:** Microcontroller sleeps most of the time, SD card fully powered off using a MOSFET when idle.
- **True Timestamps:** DS3231 RTC provides accurate current time.
- **Unique File Naming:** Each wake cycle creates a new file named using the format:
  ```
  YYMMDD_HHMM.CSV
  Example: 240426_1530.CSV
  ```
- **5 Rows of Logged Data:** Dummy sensor data + timestamps written at each cycle.

---

## 📜 Software and Libraries
- **Arduino IDE** (tested with IDE 1.8.x and 2.x)
- **Libraries Used:**
  - `SD.h` (for SD card operations)
  - `SPI.h` (SPI communication with SD card)
  - `Wire.h` (I2C communication with RTC)
  - `RTClib.h` (RTC library for DS3231)
  - `LowPower.h` (RocketScream library for deep sleep)

You can install `RTClib` and `LowPower` libraries easily via Arduino Library Manager.

---

## 👩‍💻 How It Works
1. Wake up after ~10 minutes (managed by Watchdog Timer).
2. Power ON the SD card using a GPIO-controlled P-Channel MOSFET.
3. Initialize the SD card.
4. Get the current timestamp from DS3231.
5. Create a uniquely named CSV file based on date and time.
6. Write 5 rows of dummy data (timestamp and random number).
7. Close the file.
8. Power OFF the SD card.
9. Return to deep sleep.
10. Repeat forever!

---

## 🔋 Power Consumption Estimates
| State              | Estimated Current |
|--------------------|--------------------|
| Sleep Mode         | ~10–30 μA           |
| Active (Write) Mode| ~30–60 mA (spikes up to 100 mA during SD writes) |

> **Note:** Because active time is very short (~2 seconds every 10 minutes), the average battery draw is extremely low, enabling multi-month operation.

---

## ⚡ Setup Instructions
1. **Wire the circuit** as per the architecture above.
2. **Upload** the provided `lowpowerlogging.ino` code.
3. **(Optional)** Uncomment `rtc.adjust()` line in the code once to set the RTC to compile time.
4. **Insert a FAT32-formatted microSD card** into the module.
5. **Power up** the system via a battery.
6. Watch it create timestamped CSV files automatically every 10 minutes!

---

## 💡 Notes
- Make sure to disable brown-out detector (BOD) and unused peripherals for minimum sleep current.
- Be cautious of backpowering SD card through SPI lines — hence we manually set MOSI, MISO, and SCK to INPUT after writing.
- DS3231 uses extremely little current (~1–3 μA) with backup battery.

---

## 🧪 Developed Using
- Arduino IDE
- RocketScream LowPower Library
- Brainstormed and planned manually with documentation.
- Code, circuit, and documentation refined with the help of AI for polishing explanations.

---

# 🚀 Ready to Deploy
Just power it, and let your logger run for months!

