# 🤖 Beepy – Smart Assistant Bot

Beepy is an intelligent, expressive, and interactive **smart assistant robot** built using an **Arduino-based system** (UNO/Nano) with a combination of **servo motors**, **IR sensors**, **Bluetooth control**, and a **SH1106 OLED display** for expressive eye animations. It acts as a semi-autonomous bot with a chatbot-like personality and fun robotic behaviors.

---

## 📌 Features

- 🎙️ **Bluetooth-Controlled** via voice commands (e.g., "dance")
- 👀 **Expressive OLED Eyes** that blink, look around, or appear angry
- 🦾 **Head Servo Movement** with lifelike smooth motion
- 📡 **Obstacle Detection** using IR sensors triggers “angry” expression and head turn
- 🕺 **Dance Routine** combining motor and servo movements
- 🌙 **"Alive Mode"** for idle lifelike servo movement
- 🔄 **Wake-Up Animation** with eye blinks and head scans

---

## 🧩 Components Used

| Component           | Description                         |
|---------------------|-------------------------------------|
| Arduino Nano/Uno    | Main controller                     |
| SH1106 128x64 OLED  | For animated robot eyes             |
| Servo Motor (SG90)  | Controls the head movement          |
| L298N Motor Driver  | Controls 2 gear motors              |
| 300 RPM BO Motors   | For movement                        |
| IR Sensors (x2)     | For obstacle detection (Left/Right) |
| Bluetooth Module    | HC-05 or SoftwareSerial via A4/A5   |
| Power Supply        | 7.4V Li-ion or similar              |

---

## 🔧 Pin Configuration

| Component   | Pin        |
|-------------|------------|
| Motor IN1   | D2         |
| Motor IN2   | D3         |
| Motor IN3   | D4         |
| Motor IN4   | D5         |
| ENA         | D6         |
| ENB         | D9         |
| Servo       | D12        |
| Left IR     | A0         |
| Right IR    | A1         |
| BT RX       | A4         |
| BT TX       | A5         |
| OLED CS     | D10        |
| OLED DC     | D7         |
| OLED RST    | D8         |

---

## 🧠 Functional Overview

### 🎮 Bluetooth Commands
- `"dance"` → Initiates a motor and servo dance routine

### 👁️ Eye Expressions
- **Normal Mode**: Idle eyes that randomly look around
- **Blinking**: Eyes blink every few seconds
- **Angry Mode**: Triggered by IR sensors detecting an obstacle
- **Wake-Up Animation**: Eyes blink twice + servo sweeps

### 🦾 Head Servo Movement
- Moves based on IR sensor input or random idle behavior
- Smooth transitions using incremental steps and delays

---

## 🚀 How to Use

1. Upload the code using the Arduino IDE.
2. Connect the robot via Bluetooth to your mobile app (e.g., MIT App Inventor-based Beepy controller).
3. Send the command `dance` or build voice command support.
4. Watch as Beepy responds with movements and eye animations!

---

## 🖼️ Eye Animation Logic

| Mode       | Display                           |
|------------|------------------------------------|
| Normal     | Two large rounded boxes (eyes)     |
| Blinking   | Horizontal lines (closed eyes)     |
| Angry      | Squinted eyes + angry eyebrows     |

---

## 📸 Behavior Showcase

- **Wake-Up**: Blinks twice → Sweeps head → Random head turns
- **Idle**: Eyes move in random directions every few seconds
- **Obstacle**: When IR sensor detects something, switches to angry face + turns head
- **Dance**: Moves motors forward and backward + servo side to side

---

## 📄 Libraries Used

- [`U8g2`](https://github.com/olikraus/u8g2) – for OLED animations
- `Servo` – for SG90 control
- `SoftwareSerial` – for Bluetooth communication

Install them via **Library Manager** in Arduino IDE before uploading.

---

## 🔋 Power Tips

- Power the motors separately using a battery (Li-ion or 7.4V pack)
- Use a common GND between motor driver, Arduino, and battery
- Add capacitors if flickering occurs with OLED or servo

---

## 📱 Recommended App

You can use **MIT App Inventor** to create a simple app with:
- Bluetooth connection
- Voice-to-text command (e.g., "dance")
- Status display

---

## 💡 Future Ideas

- Add more voice commands (like "sing", "move around")
- Integrate a sound module for voice or music playback
- Add ultrasonic for better obstacle sensing
- Connect to AI chatbot for intelligent responses

---

## 🙋 Author

**Name**: Karan Sharma
**Email**: karanx11.72898@gmail.com
**Institute**: BBDITM, Lucknow  
**Domain**: B.Tech CSE (AI & ML)

---

## 🛡️ License

Open-source for educational use only. Please give credit when used.

---

