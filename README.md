# ESP32 WiFi Server with Two-Client Control System

This repository contains an ESP32-based project that creates a WiFi access point and server to connect two clients. 
Each client can send data to the server, and the server controls an LCD screen and a buzzer based on received data and button states.

## Hardware Requirements

- 1 x ESP32
- 1 x LCD with I2C module
- 1 x Buzzer
- 1 x Button
- 2 x Sensors (for clients)
- Jumper wires

## Software Requirements

- Arduino IDE (for ESP32 programming)
- WiFi and LiquidCrystal_I2C libraries

## Circuit Setup

1. **Server ESP32**: Connect the LCD to the I2C pins (SDA and SCL), the button to `BUTTON_PIN` (GPIO 4), and the buzzer to `BUZZER_PIN` (GPIO 12).
2. **Client ESP32 Devices**: Each client needs a sensor connected to `sensorPin` (GPIO 13) and a buzzer connected to `buzzerPin` (GPIO 12).

## Code Overview

### Server Code

The server code (`server.ino`) sets up a WiFi access point with a specified SSID and password. It creates a TCP server and listens for connections on port 8080.

- **WiFi Setup**: The server creates an access point with the SSID `ESP32_AP` and password `password`.
- **Client Connection**: Waits for two clients to connect. When each client connects, the server gives a visual and audible signal by blinking the buzzer three times.
- **Button State**: The button on the server device is used to toggle a "lock" state.
- **Client Communication**: When a client sends data, the server reads and prints it to the Serial Monitor if the system is not in "lock" state.

### Client Code

The client code (`client.ino`) connects to the server and sends data when a sensor is triggered.

- **WiFi Setup**: Each client connects to the access point `ESP32_AP` with password `password` and attempts to connect to the server IP `192.168.4.1` on port 8080.
- **Sensor and Buzzer**: When the sensor is activated (set to LOW), the client sends the ASCII character `65` (`'A'`) to the server, activates the buzzer, and waits until the sensor is deactivated.

### Communication Protocol

Each client sends the character `'A'` when its sensor is triggered. However, depending on the client, the data can be modified (e.g., `65` for Client 1 and `98` for Client 2). This differentiation allows the server to handle each client independently based on its input.

## Usage

1. Upload the `server.ino` to the ESP32 server device and the `client.ino` to each client device.
2. Power up all devices. The server will create a WiFi access point with SSID `ESP32_AP` and password `password`.
3. When clients connect to the server, it will display "Ready" on the LCD.
4. If the button on the server is pressed, the system will enter "lock" mode, and data from clients will be ignored until "lock" mode is released.

## Future Improvements

- Add more clients by expanding client array size and adjusting server loop to handle more connections.
- Implement encryption for data transfer between server and clients for secure communication.
- Add an OLED display for more detailed information.
