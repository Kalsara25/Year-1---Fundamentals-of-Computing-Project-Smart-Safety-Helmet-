# Smart Safety Helmet 🚨🪖
**Year 1 - Fundamentals of Computing Project**

![License](https://img.shields.io/badge/License-MIT-green.svg)
![Build](https://img.shields.io/badge/Build-Passing-brightgreen.svg)
![Status](https://img.shields.io/badge/Status-Active-blue.svg)
![Made With](https://img.shields.io/badge/Made%20With-React%20%7C%20Firebase%20%7C%20ESP32-orange.svg)
![Contributions](https://img.shields.io/badge/Contributions-Welcome-brightgreen.svg)
![Repo Size](https://img.shields.io/github/repo-size/your-username/year1-fundamentals-smart-safety-helmet)
![Last Commit](https://img.shields.io/github/last-commit/your-username/year1-fundamentals-smart-safety-helmet)

The Smart Safety Helmet is an IoT-based safety solution developed as part of the Year 1 "Fundamentals of Computing" module. It integrates hardware sensors with modern web technologies to provide **real-time monitoring, instant alerts, and improved worker safety**.

---

## 🔑 Key Features
- ✅ Helmet usage detection (helmet worn/not worn)
- ✅ Gas leakage monitoring (MQ sensors)
- ✅ Temperature & humidity tracking
- ✅ Fall detection for worker safety
- ✅ Heart rate monitoring
- ✅ Real-time data with **Firebase Realtime Database**
- ✅ Web dashboard with **React.js** (3D-style, responsive UI)
- ✅ Visual indicators (red/green bulbs) & instant alerts
- ✅ ESP32 microcontroller for IoT connectivity

---

## 🏗️ Tech Stack
- **Frontend:** React.js  
- **Backend/Database:** Firebase Realtime Database  
- **Hardware:** ESP32, MQ135 Gas Sensor, DHT22 Temp & Humidity Sensor, Heart Rate Sensor, IR Sensor  
- **Languages:** JavaScript, C++ (Arduino IDE)

---

## ⚙️ Installation & Setup
Navigate to project folder:
```bash
cd year1-fundamentals-smart-safety-helmet

Install dependencies (for React app):

npm install


Start development server:

npm start

📊 Project Structure
/helmet-project
 ├── frontend/        # React Web Application
 ├── esp32-code/      # ESP32 Sensor Integration
 ├── firebase/        # Firebase Config Files
 ├── assets/          # Images & Resources
 └── README.md        # Project Documentation

🚀 Future Improvements

AI-based predictive safety alerts

Mobile application integration

Cloud-based analytics dashboard

Multi-user authentication system

## 📑 Documentation
[📄 Download Project Documentation (PDF)](docs/SmartHelmet-Documentation.pdf)

📸 Screenshots

![Helmet Dashboard](assets/images/dashboard.png)

## 🖥️ ESP32 Arduino Code

The `esp32-code/` folder contains the Arduino source code for the Smart Safety Helmet project.

- **helmet_main.ino** → Main Arduino sketch for ESP32
- **sensors.cpp** → Sensor handling logic (Gas, Temp, Humidity, Heart rate, Fall detection)
- **sensors.h** → Header file for sensor definitions

To upload:
1. Open `helmet_main.ino` in Arduino IDE
2. Select **Board: ESP32 Dev Module**
3. Connect helmet ESP32 via USB
4. Click **Upload**


📜 License

This project is developed for educational purposes as part of the Year 1 - Fundamentals of Computing module.
You are free to use and modify it with proper attribution.


   ```bash
   git clone https://github.com/<your-username>/year1-fundamentals-smart-safety-helmet.git
