# S.P.Y.D.E.R 🕷️
### Spider-like Prototype Yields Data for Exploration and Research

<p align="center">
  <img src="https://img.shields.io/badge/Platform-Arduino%20Nano-00979D?style=for-the-badge&logo=arduino&logoColor=white"/>
  <img src="https://img.shields.io/badge/Platform-ESP32--CAM-E7352C?style=for-the-badge&logo=espressif&logoColor=white"/>
  <img src="https://img.shields.io/badge/Control-Blynk%20IoT-00C200?style=for-the-badge"/>
  <img src="https://img.shields.io/badge/Course-CSE%20342-blue?style=for-the-badge"/>
  <img src="https://img.shields.io/badge/University-East%20Delta%20University-8B0000?style=for-the-badge"/>
</p>

---

## 📖 Overview

**S.P.Y.D.E.R** is an arachnid-inspired quadruped robot designed for remote surveillance, exploration, and research in environments that are inaccessible or hazardous to humans. The robot features **12 servo-driven legs** that replicate the natural locomotion of a spider, enabling stable traversal across rough and uneven terrain. It is controlled wirelessly over **Wi-Fi via the Blynk IoT platform** and streams **live video feed** through an onboard **ESP32-CAM (OV2640)** module.

This project was developed as part of **CSE 342: IoT-based Project Development** at the School of Science, Engineering and Technology, **East Delta University**.

---

## ✨ Features

- 🦿 **12-DOF Quadruped Locomotion** — Four legs, three servo motors each, for fluid arachnid-style movement
- 📡 **Wi-Fi Remote Control** — Wireless control via the Blynk IoT mobile application
- 📷 **Live Video Streaming** — Real-time MJPEG video stream served over HTTP via ESP32-CAM
- 🔄 **Dual Microcontroller Architecture** — ESP32-CAM handles wireless communication; Arduino Nano handles motor control
- ⚡ **Stable Power Management** — LM2596 buck converter regulates voltage for all servo motors at a constant 5V
- 🖨️ **3D-Printed Chassis** — Lightweight and rigid exoskeleton designed for optimal component mounting and movement
- 🎮 **Multiple Movement Modes** — Forward, Backward, Left, Right, Wave, Dance, Stand, Sit

---

## 🗂️ Repository Structure

```
SPYDER/
│
├── 1.legs/
│   └── Legs.ino                        # Servo initialization & leg calibration
│
├── 2.esp32_blynk_camera3/
│   └── esp32_blynk_camera3.ino         # ESP32-CAM: Wi-Fi, Blynk & live video stream
│
└── 3.remote3/
    └── spider_remote_blynk3/
        ├── spider_remote_blynk3.ino    # Arduino Nano: movement control & IK engine
        ├── FlexiTimer2.cpp             # Timer interrupt library for servo sync
        └── FlexiTimer2.h
```

---

## 🔧 Hardware Components

| # | Component | Purpose |
|---|-----------|---------|
| 1 | **ESP32-CAM (OV2640)** | Live video streaming + Wi-Fi remote control |
| 2 | **ESP32 Programming Board** | Flashing firmware onto the ESP32-CAM |
| 3 | **Arduino Nano** | Central motor controller & movement logic |
| 4 | **Arduino Nano 328P Expansion Board** | Breakout board for 12 servo signal connections |
| 5 | **SG90 Mini Servo Motors × 12** | Three per leg for full 3-DOF leg articulation |
| 6 | **LM2596 Buck Converter** | Step-down voltage regulation (5V for all servos) |
| 7 | **3D-Printed Body** | Structural chassis for all components |
| 8 | **Jumper Wires & Battery Pack** | Wiring and power supply |

---

## 🏗️ System Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                        Blynk App (Mobile)                   │
│         V0:Fwd  V1:Back  V2:Left  V3:Right                  │
│         V4:Wave V5:Dance V6:Stand V7:Sit   V8:CamIP         │
└────────────────────────┬────────────────────────────────────┘
                         │ Wi-Fi
                         ▼
              ┌──────────────────────┐
              │   ESP32-CAM (OV2640) │
              │  • Blynk IoT Client  │
              │  • HTTP Stream (/:80)│
              │  • Serial Bridge     │
              └──────────┬───────────┘
                         │ UART (115200 baud)
                         │ Command chars: F/B/L/R/W/D/S/C
                         ▼
              ┌──────────────────────┐
              │    Arduino Nano      │
              │  • Inverse Kinematics│
              │  • Gait Sequencing   │
              │  • FlexiTimer2 (50Hz)│
              └──────────┬───────────┘
                         │ PWM signals
                         ▼
         ┌───────────────────────────────┐
         │   12 × SG90 Servo Motors       │
         │  Leg 0: pins 2,3,4            │
         │  Leg 1: pins 5,6,7            │
         │  Leg 2: pins 8,9,10           │
         │  Leg 3: pins 11,12,13         │
         └───────────────────────────────┘
```

---

## 💻 Software Setup

### Prerequisites

- [Arduino IDE](https://www.arduino.cc/en/software) (v1.8+ or v2.x)
- [Blynk IoT App](https://blynk.io/) (mobile — iOS or Android)
- ESP32 Board Package installed in Arduino IDE
- Required Libraries:
  - `Servo.h` (built-in)
  - `BlynkSimpleEsp32`
  - `esp_camera.h` (ESP32 core)
  - `WiFi.h` (ESP32 core)
  - `WebServer.h` (ESP32 core)
  - `FlexiTimer2` (included in repo)

### Installation

**1. Clone the repository**
```bash
git clone https://github.com/your-username/SPYDER.git
cd SPYDER
```

**2. Flash the Arduino Nano**

Open `3.remote3/spider_remote_blynk3/spider_remote_blynk3.ino` in Arduino IDE.

- Board: `Arduino Nano`
- Processor: `ATmega328P`
- Upload the sketch.

**3. Configure Wi-Fi & Blynk credentials**

In `2.esp32_blynk_camera3/esp32_blynk_camera3.ino`, update the following:

```cpp
char auth[] = "YOUR_BLYNK_AUTH_TOKEN";
char ssid[] = "YOUR_WIFI_SSID";
char pass[] = "YOUR_WIFI_PASSWORD";
```

**4. Flash the ESP32-CAM**

- Connect the ESP32-CAM via the ESP32 Programming Board.
- Board: `AI Thinker ESP32-CAM`
- Upload `2.esp32_blynk_camera3/esp32_blynk_camera3.ino`.

**5. Calibrate servo positions (optional)**

Open `1.legs/Legs.ino` and upload to the Arduino Nano to set all servos to 90° (neutral position) before final assembly.

---

## 📱 Blynk App Configuration

Create a new template in the Blynk IoT console and map the following virtual pins:

| Virtual Pin | Action | Command Sent |
|-------------|--------|-------------|
| `V0` | Forward | `'F'` |
| `V1` | Backward | `'B'` |
| `V2` | Turn Left | `'L'` |
| `V3` | Turn Right | `'R'` |
| `V4` | Wave | `'W'` |
| `V5` | Dance | `'D'` |
| `V6` | Stand | `'S'` |
| `V7` | Sit | `'C'` |
| `V8` | Camera IP Display | *(text widget — auto-populated)* |

Once connected, the ESP32-CAM will automatically push its IP address to the V8 widget. Open the displayed URL in any browser on the same network to view the live video stream.

---

## 📐 Inverse Kinematics

The robot's movement engine uses a 3-DOF inverse kinematics model per leg. Key physical parameters:

```cpp
const float length_a    = 55;    // Coxa length (mm)
const float length_b    = 77.5;  // Femur length (mm)
const float length_c    = 27.5;  // Tibia length (mm)
const float length_side = 71;    // Body half-width (mm)
const float z_absolute  = -28;   // Absolute floor height (mm)
```

Movement is managed by a **FlexiTimer2** interrupt running at **50 Hz (20ms)**, which smoothly interpolates each leg's end-effector position from its current coordinates to the target coordinates using per-axis velocity calculation.

---

## 🔬 Evaluation Metrics

| Metric | Description |
|--------|-------------|
| **Mobility Efficiency** | Ability to traverse rough terrain, climb small obstacles, and maintain balance |
| **Control Latency** | Round-trip delay between Blynk button press and motor response over Wi-Fi |
| **Video Stream Quality** | Frame rate, JPEG quality, and stability under varying lighting and network conditions |
| **Power Stability** | Voltage consistency at servo rails during peak load |

---

## 🚀 Future Enhancements

- [ ] Autonomous navigation using ultrasonic / LiDAR sensors
- [ ] AI-based computer vision for object detection and path planning
- [ ] Extended wireless range via MQTT over the internet
- [ ] Radiation / environmental sensor integration
- [ ] Improved gait algorithm for hexapod (6-leg) expansion
- [ ] Obstacle avoidance with real-time terrain mapping

---

## 👥 Authors

| Name | Student ID |
|------|-----------|
| Prottoy Barua | 223018112 |
| Avraw Das | 223016612 |
| Shaswata Das | 223016312 |
| Adnan Sameer | 223017912 |

**Course:** CSE 342 — IoT-based Project Development, Section 1  
**Institution:** School of Science, Engineering and Technology, East Delta University

---

## 📄 License

This project is developed for academic purposes at East Delta University. All rights reserved by the respective authors.

---

<p align="center">
  <i>Built with curiosity, caffeine, and 12 tiny servo motors. 🕷️</i>
</p>