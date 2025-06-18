# 🚀 Rocket Beacon — ATTiny85 Morse Code Transmitter

This project uses an ATTiny85 microcontroller to transmit a repeating message in Morse code via a digital output pin (e.g., connected to an LED or RF transmitter). The system enters deep sleep between transmissions to minimize power consumption, making it suitable for beacon-like applications such as balloons, rockets, or remote sensors.

---

## 🔧 Features

- ✅ Morse code transmission of customizable messages  
- ✅ Optimized power consumption using AVR `sleep()` and watchdog timer  
- ✅ Configurable speed and timing for dots/dashes  
- ✅ Easily extensible for LEDs, buzzers, or RF modules  
- ✅ Built on PlatformIO for flexible development

---

## 🛠 Hardware

- [ ] **Microcontroller:** ATTiny85  
- [ ] **Output:** RF transmitter (connected to `PB1`, physical pin 6)
- [ ] **Power Supply:** Coin cell battery, LiPo, or regulated 3–5V source  
- [ ] **Programming:** USBasp, USBtinyISP, or Arduino UNO configured as an ISP programmer

---

## 🔁 How It Works

1. The `sendmsg()` function encodes the input message into Morse code.
2. The message is output via `digitalWrite()` on `PB1` (pin 6).
3. After transmission, the ATTiny85 enters deep power-down sleep mode.
4. The watchdog timer wakes the chip every 1 second until the total sleep time (e.g., 15 seconds) is completed.
5. The message transmission repeats in a loop.

---

## 🧪 Example Transmission

**Message:**  
`VK2ASN VK2ASN VK2ASN TESTING TESTING`

**Output:**  
Morse tones or flashes repeating every 15 seconds.

---

## 💻 Getting Started (PlatformIO)

### 1. Clone the repository

```
git clone https://github.com/abhi-243/Rocket-Beacon---ATTiny85.git
cd Rocket-Beacon---ATTiny85
```
### 2. Connect and Program the ATTiny85 Using Arduino as ISP
If you don’t have a dedicated ISP programmer, you can use an Arduino UNO as a programmer:

Connect Arduino UNO pins to ATTiny85 as follows:

|Arduino UNO Pin| ATTiny85 Pin |Description|
|---------------|--------------|-----------|
|10	            |1 (RESET)     |RESET      |
|11	            |5 (MOSI)      |MOSI       |
|12	            |6 (MISO)	   |MISO       |
|13	            |7 (SCK)       |SCK        |
|5V	            |VCC (Pin 8)   |Power      |
|GND            |GND (Pin 4)   |Ground     |

On Arduino IDE, upload the ArduinoISP sketch (File > Examples > 11.ArduinoISP > ArduinoISP).

Modify ```platformio.ini``` to use the ```programmer = arduino``` option and select the correct serial port.

Upload the firmware with:
```pio run -t upload```



### 3.Build and Upload via PlatformIO

Open ```platformio.ini``` and verify your programmer settings and serial port.

Run the upload command:
```
pio run -t upload
```

### ⚙️ Configuration
- ```txpin```: Output pin (1 — physical pin 6 on ATTiny85)

- ```SPEED```: Morse code speed in WPM (default 13)

- ```sleepFor(ms)```: Duration to sleep between message repeats (default 15000 ms)

### 🔋 Power Optimization
- ADC, timers, and unused peripherals disabled to reduce power
- Uses the deepest sleep mode ```(SLEEP_MODE_PWR_DOWN)```
- Watchdog timer interrupt wakes device in 1-second intervals

### 🛠 Dependencies
No external libraries required. Uses AVR native headers:

```
avr/sleep.h
avr/power.h
avr/wdt.h
```
