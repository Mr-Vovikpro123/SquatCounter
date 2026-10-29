# SquatCounter — STM32 Motion-Based Rep Counter

An embedded squat counter built using an **STM32F103C8T6 (Blue Pill)** microcontroller, an **MPU6050 accelerometer/gyroscope**, and a **128×64 OLED display**.

The device detects squat movements in real time by analyzing acceleration and angular velocity data, automatically counting completed repetitions.

## Features

- **Motion detection:** Reads accelerometer and gyroscope data from the MPU6050 via I²C.
- **Finite State Machine (FSM):** Tracks four movement stages to detect complete repetitions.
- **Real-time feedback:** Displays the repetition count, current movement state, and acceleration data on an SSD1306 OLED.
- **LED indication:** Uses an onboard LED to indicate movement states.
- **Threshold-based detection:** Identifies squat phases without requiring complex signal processing.

## Hardware

| Component | Description |
|---|---|
| Microcontroller | STM32F103C8T6 (Blue Pill) |
| Motion sensor | MPU6050 (3-axis accelerometer + gyroscope) |
| Display | SSD1306 OLED, 128×64 |
| Communication | I²C |

## How It Works

The firmware implements a four-state Finite State Machine:

1. **STAND:** Initial standing position.
2. **GOING_DOWN:** Downward movement detected using angular velocity.
3. **BOTTOM:** Bottom position detected using acceleration thresholds.
4. **GOING_UP:** Upward movement detected.

A repetition is counted only when the system returns to the standing position after completing the entire movement sequence.

## Software

The firmware is written in **C**, using the **STM32 HAL library** and developed in **STM32CubeIDE**.

The project demonstrates practical implementation of sensor interfacing, real-time motion detection, and state-based embedded control.
