# ARDUINO-UNO Project Setup Guide

This guide provides step-by-step instructions for setting up the environment and installing the required tools to work with the AVR microcontroller project.

## Folder Structure

The project contains the following main folders:

1. **`avr_tools/`**
   - Contains the AVR compiler, linker, and other necessary utilities.
   - Example path: `avr_tools/avr8-gnu-toolchain-win32_x86_64/bin`

2. **`avrdude-v8.0-windows-x64/`**
   - Contains the AVRDUDE tool for uploading binary to the microcontroller.

3. **`docs/`**
   - Contains documentation related to Arduino Uno and ATmega328P.

## Environment Setup

Follow these steps to set up the environment:

### 1. Download Tools
- Ensure you have the following folders:
  - `avr_tools`  Download avr-tools as per your Operating system. Make sure to download 8-bit toolchain
  https://www.microchip.com/en-us/tools-resources/develop/microchip-studio/gcc-compilers
  - `avrdude-v8.0-windows-x64`
  - `docs`

### 2. Add Paths to Environment Variables

You need to add the following paths to your system’s **Environment Variables**:

#### Add to `PATH`:
1. **Path for AVR tools:**
   ```
   <project_root>/avr_tools/avr8-gnu-toolchain-win32_x86_64/bin
   ```

2. **Path for AVRDUDE tools:**
   ```
   <project_root>/avrdude-v8.0-windows-x64
   ```

### 3. Verify Installation

#### Verify AVR Tools:
Run the following command in a terminal to confirm the AVR tools are correctly installed:
```bash
avr-gcc --version
```
You should see the version information for AVR GCC.

#### Verify AVRDUDE:
Run the following command to confirm AVRDUDE is correctly installed:
```bash
avrdude --version
```
You should see the version information for AVRDUDE.

### 4. Documentation
- Refer to the `docs/` folder for detailed information about the Arduino Uno and ATmega328P, including datasheets and reference manuals.

## Notes
- Ensure the paths to `avr_tools` and `avrdude-v8.0-windows-x64` are added to the **system-wide** environment variables for seamless use.
- If you encounter issues with permissions, make sure to run your terminal or IDE with administrator privileges.

## Example Commands
Here are some example commands you might find useful:

- **Compiling Source Code:**
  ```bash
  avr-gcc -mmcu=atmega328p -o output.elf source.c
  ```

- **Uploading Firmware:**
  ```bash
  avrdude -F -V -c arduino -p atmega328p -P COM8 -b 115200 -U flash:w:output.hex
  ```

Happy Coding!

