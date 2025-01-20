
# LabJack Temperature Monitoring Program

## Overview
This C program allows you to interface with a **LabJack U3** device to monitor temperature readings from an analog input (AIN1). It compares the current temperature against a user-defined threshold and activates a buzzer and an LED indicator based on the results. The program can exit based on either a **time-based** condition or a **buzzer activation count**.

## Features
- Monitors temperature readings from a connected sensor.
- Activates a buzzer and turns off an LED when the temperature exceeds a threshold.
- Flexible exit strategies: exit based on time or the number of buzzer activations.

## Prerequisites

- **LabJack U3**: A LabJack U3 device is required.
- **LabJack Drivers and LabJackUD Library**: Ensure the LabJack drivers and the `LabJackUD` library are installed. These can be downloaded from the official [LabJack website](https://labjack.com/support/software).
- **Windows Operating System**: This code is designed for Windows and uses the Windows API (`Windows.h`).
- **C Compiler**: A C compiler such as GCC.

## Installation

### 1. **LabJack Library Installation**
   - Download the **LabJackUD** driver from the [LabJack Support page](https://labjack.com/support/software).
   - Install the LabJack drivers on your system.
   - Ensure the LabJack library files (e.g., `LabJackUD.h`) are available and accessible during compilation.

### 2. **Setting up the LabJackUD Library for Compilation**
   If you're using GCC or a similar compiler, you can compile the program with the following flags:
   ```bash
   gcc -o labjack_temperature_monitor labjack_temperature_monitor.c -L"C:\Program Files (x86)\LabJack\Drivers" -lLabJackUD
   ```
   Ensure that the path to `LabJackUD.h` is set correctly in your environment or is referenced explicitly during the compilation.

## Usage

1. **Connect your LabJack U3 device** to your system via USB.
2. **Connect the temperature sensor** to the **AIN1** input channel of the LabJack device.
3. **Compile and run the program**:
   ```bash
   ./labjack_temperature_monitor
   ```

4. **Program Flow**:
   - **Threshold Input**: Enter the desired temperature threshold (in Celsius).
   - **Exit Strategy**: Choose an exit strategy:
     - **Time-based**: The program will stop after a set duration (in seconds).
     - **Buzzer Activation Count**: The program will stop after a specified number of buzzer activations.
   - **Temperature Monitoring**: The program will continuously monitor the temperature, activate the buzzer if the threshold is exceeded, and provide output messages.

### Example Interaction
```
Welcome to the temperature monitoring program!

Enter the threshold limit in Celsius: 25
Choose exit strategy:
1. Time-based
2. Buzzer activation count
Your choice: 1
Enter the duration of program in seconds: 10

Current temperature: 23.21 C
Temperature is NORMAL. LED ON.
Current temperature: 22.15 C
Temperature is NORMAL. LED ON.
Current temperature: 25.32 C
Temperature is Above Threshold | Buzzer activated.
Current temperature: 26.84 C
Temperature is Above Threshold | Buzzer activated.
Program ended.
```

## Code Overview

- **Initialization**:
  - The program opens a connection with the LabJack U3 device and configures the necessary output channels (buzzer and LED).
  - The threshold temperature value and exit strategy are defined by the user.
  
- **Monitoring**:
  - Continuously reads the temperature from the sensor and checks if it exceeds the defined threshold.
  - If the temperature is above the threshold:
    - The LED is turned off.
    - The buzzer is activated.
  - If the temperature is normal:
    - The LED is turned on, and the buzzer is turned off.
  
- **Exit Condition**:
  - The program will stop based on the exit strategy selected by the user (either time-based or buzzer activation count).

## Notes

- **Temperature Conversion**: The program assumes a linear relationship where 1V corresponds to 100°C.
- **System Requirements**: This program is designed for **Windows OS** and utilizes `Windows.h` for timing functions (such as `Sleep()`).
  
## Troubleshooting

- **Device Not Found**: Ensure that the LabJack device is connected properly via USB, and that the LabJack drivers are correctly installed.
- **Incorrect Temperature Readings**: Verify that the temperature sensor is connected properly and is functioning correctly.

## Acknowledgements
- **LabJack U3** hardware and **LabJackUD library**.
- [LabJack Documentation](https://labjack.com/support).

