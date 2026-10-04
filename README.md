# Retro Snake Game — Embedded Systems Implementation
<img width="800" height="600" alt="image" src="https://github.com/user-attachments/assets/d4f6584f-11fe-4214-abb8-24be26dba060" />


An asynchronous, non-blocking implementation of the classic Snake arcade game engineered for microcontrollers (ESP32 / Arduino). The system features hardware I2C interfacing for an SSD1306 OLED display, active-low tactile button navigation, and dynamic PWM-based auditory feedback.


## Technical Overview & Architecture

Unlike basic educational sketches that rely on blocking calls (`delay()`), this firmware is built around deterministic timing and robust state machine principles:

* **Non-Blocking Execution Loop:** Game update cycles and frame rendering rely on `millis()` delta-timing (`gameSpeed`), ensuring responsive input sampling and consistent tick intervals.
* **Low-Latency Polling:** Button inputs are configured with internal pull-up resistors (`INPUT_PULLUP`), eliminating floating pin states and the need for external pull-up resistor networks.
* **Direction Guarding:** Reversal-prevention logic prevents the snake from performing illegal 180-degree immediate turns into its own neck (e.g., UP is ignored when currently moving DOWN).
* **Collision Detection Engine:** Real-time array scanning detects self-intersections with tail segments and boundary collisions with the $128 \times 64$ screen perimeter.
* **Acoustic Feedback Engine:** Variable-frequency square waves driven via a passive buzzer provide clear event distinction: 1000 Hz (Game Init), 1500 Hz (Food Consumed), and 300 Hz (Collision / Game Over).


## Hardware Specifications

| Component | Specification | Interface |
| :--- | :--- | :--- |
| **Microcontroller** | ESP32 DevKit v1 / Arduino Compatible | GPIO, I2C, ADC |
| **Display** | 0.96" Monochrome SSD1306 OLED (128x64 px) | Hardware I2C (Address `0x3C`) |
| **Inputs** | 4x Momentary Tactile Push Buttons | Digital Inputs (Internal Pull-Up) |
| **Audio Feedback** | 5V Passive Buzzer | PWM / Digital Output |


## Pinout & Wiring Diagram

| Function | Pin Name | Default Board Pin | Electrical Notes |
| :--- | :--- | :--- | :--- |
| **I2C SDA** | Serial Data | GPIO 21 / A4 | Connected to OLED SDA line |
| **I2C SCL** | Serial Clock | GPIO 22 / A5 | Connected to OLED SCL line |
| **BTN_UP** | Navigate Up | GPIO 2 | Active LOW (Switched to GND) |
| **BTN_DOWN** | Navigate Down | GPIO 4 | Active LOW (Switched to GND) |
| **BTN_LEFT** | Navigate Left | GPIO 16 | Active LOW (Switched to GND) |
| **BTN_RIGHT**| Navigate Right | GPIO 17 | Active LOW (Switched to GND) |
| **BUZZER** | Audio Output | GPIO 15 | Driven via 100-220Ω current-limiting resistor |
| **RNG Seed** | Floating ADC | GPIO 34 / A0 | Samples floating analog noise for `randomSeed()` |


## Dependencies & Libraries

Ensure the following libraries are installed in your Arduino IDE or PlatformIO environment:

* **Adafruit SSD1306** (`>= 2.5.0`) — Display driver for monochrome 128x64 panels.
* **Adafruit GFX Library** (`>= 1.11.0`) — Graphics core library for primitives.
* **Wire** — Standard I2C communication library.


## Installation & Setup

1. **Clone the repository:**
   ```
   git clone https://github.com/gaedarius3/ESP32-Snake.git
   ```

2. **Open the project:**
   * Open the sketch file in **Arduino IDE** or **VS Code with PlatformIO**.

3. **Install Dependencies:**
   * In Arduino IDE: Navigate to `Tools` -> `Manage Libraries...`, search for `Adafruit SSD1306` and `Adafruit GFX`, and install all dependencies.

4. **Connect Hardware:**
   * Wire the I2C OLED display (VCC, GND, SDA, SCL).
   * Connect each push button between its designated GPIO and board GND.
   * Connect the buzzer between GPIO 15 and GND.

5. **Flash the Firmware:**
   * Select your board model and COM port under the `Tools` menu.
   * Click **Upload**.


## Controls & Gameplay

* **Directional Movement:** Use the **UP**, **DOWN**, **LEFT**, and **RIGHT** push buttons to steer the snake.
* **Scoring:** Each eaten food item increases score by 5 points and extends the snake's tail length.
* **Game Over:** Crashing into walls or running into the snake's body terminates the run.
* **Restart:** Press the **UP** button while on the Game Over screen to reinitialize a new round.


## Engineering Roadmap

- [ ] **Dual-Core Task Separation:** Leverage FreeRTOS on ESP32 to run game logic and display rendering on independent cores.
- [ ] **Persistent High Scores:** Store personal best scores in non-volatile memory (EEPROM / NVS).
- [ ] **Dynamic Difficulty Scaling:** Incrementally decrease `gameSpeed` interval as the snake grows.
- [ ] **Input FIFO Queue:** Implement an input buffer to prevent illegal corner-trap collisions during rapid double-key presses.


