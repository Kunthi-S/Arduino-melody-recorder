# Arduino-melody-recorder# Melody Recorder

A simple Arduino-based melody recorder and player using a **4×4 keypad**, **passive buzzer**, and **0.96" SSD1306 OLED display**. Users can play notes, record short melodies, adjust the octave, and replay recorded sequences.

## Features

* Play musical notes using the keypad
* Record up to **50 notes**
* Playback recorded melodies while preserving timing between notes
* Octave adjustment (−2 to +2)
* OLED status display
* Clear recorded melody
* Simple and lightweight implementation

---

## Hardware Requirements

* Arduino Uno (or compatible)
* 4×4 Matrix Keypad
* Passive Buzzer
* 0.96" I2C SSD1306 OLED Display (128×64)
* Jumper wires
* Breadboard (optional)

---

## Pin Connections

### OLED (I2C)

| OLED | Arduino |
| ---- | ------- |
| VCC  | 5V      |
| GND  | GND     |
| SDA  | A4      |
| SCL  | A5      |

### Passive Buzzer

| Buzzer | Arduino |
| ------ | ------- |
| +      | D2      |
| -      | GND     |

### 4×4 Keypad

| Keypad Pin | Arduino Pin |
| ---------- | ----------- |
| R1         | D10         |
| R2         | D9          |
| R3         | D8          |
| R4         | D7          |
| C1         | D6          |
| C2         | D5          |
| C3         | D4          |
| C4         | D3          |

---

## Key Functions

### Musical Notes

| Key | Note |
| --- | ---- |
| 1   | C4   |
| 2   | D4   |
| 3   | E4   |
| 4   | F4   |
| 5   | G4   |
| 6   | A4   |
| 7   | B4   |
| 8   | C5   |
| 9   | D5   |

---

### Control Keys

| Key | Function                                    |
| --- | ------------------------------------------- |
| A   | Start recording (clears previous recording) |
| B   | Stop recording                              |
| C   | Playback recorded melody                    |
| D   | Clear recording                             |
| *   | Octave down                                 |
| #   | Octave up                                   |

---

## OLED Display

The OLED displays:

* Current mode (READY, REC, PLAY, DONE, etc.)
* Current note being played
* Current octave
* Number of recorded notes

Example:

```
REC          O:1

G

Len:12
```

---

## Recording

1. Press **A** to start recording.
2. Play notes using keys **1–9**.
3. Press **B** to stop recording.
4. Press **C** to replay the melody.
5. Press **D** to erase the recording.

The recorder stores:

* Note frequency
* Time interval between consecutive notes

Maximum recording length: **50 notes**.

---

## Octave Adjustment

Use:

* `*` → Lower octave
* `#` → Raise octave

Supported range:

* Octave −2
* Octave −1
* Octave 0 (default)
* Octave +1
* Octave +2

---

## Arduino Libraries

Install the following libraries using the Arduino Library Manager:

* Adafruit GFX Library
* Adafruit SSD1306
* Keypad

The project also uses the built-in:

* Wire library

---

## Project Structure

```
Melody_Recorder.ino
│
├── OLED display functions
├── Keypad input
├── Note mapping
├── Recording system
├── Playback system
├── Octave control
└── Main program loop
```

---

## Future Improvements

* Save melodies to EEPROM
* Adjustable playback tempo
* Longer recording capacity
* Metronome support
* Multiple melody storage
* LCD/OLED menu interface

---

## Author

Developed as an Arduino melody recorder project for learning embedded systems, keypad interfacing, sound generation, and OLED display programming.
