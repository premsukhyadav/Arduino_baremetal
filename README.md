# Arduino Baremetal Programming

## Table of Contents
1. [01_blink_led](01_blink_led)
2. [02_register_ReadWrite](02_register_ReadWrite)
3. [03_delay_timer](03_delay_timer)

## Introduction to Baremetal Programming
Baremetal programming refers to writing software that interacts directly with the hardware without an operating system or high-level abstractions. It is particularly important in embedded systems for achieving:

- **High Efficiency**: Direct control over hardware ensures optimal use of system resources.
- **Low Latency**: Immediate response to hardware events.
- **Customizability**: Tailored software solutions for specific hardware.

By understanding baremetal principles, developers can:
- Gain deeper insights into microcontroller architecture.
- Write highly efficient, resource-optimized code.
- Build foundational knowledge essential for advanced embedded systems development.

## Why Arduino for Baremetal Programming?
Arduino boards, particularly the Arduino Uno, are excellent starting points for baremetal programming due to their:

1. **Simplicity**: Minimalistic design makes them beginner-friendly.
2. **Rich Documentation**: Ample community and official resources for reference.
3. **Cost-Effectiveness**: Affordable hardware for experiments and learning.
4. **Bootloader**: Includes a bootloader that simplifies flashing applications without needing external programmers (though you can bypass it).

### Bootloader and Application in Arduino Uno
- **Bootloader**: The Arduino Uno comes pre-loaded with a bootloader in the upper memory of its flash (addresses starting near `0x7800` for ATmega328P). The bootloader enables programming via USB without an external programmer.
- **Application Code**: When you upload a sketch, it occupies flash memory starting from `0x0000`.

The bootloader executes first and determines whether to run the user application or wait for new firmware to be uploaded. This simplifies development while preserving baremetal flexibility.

### Why Transition to Baremetal from Arduino IDE?
The Arduino ecosystem provides an abstraction over microcontroller hardware to speed up prototyping. While convenient, this abstraction:
- Limits control over hardware.
- Hides important details such as register manipulation.

Baremetal programming removes these limitations, helping developers:
- Write highly optimized code.
- Understand hardware registers and peripherals intimately.
- Develop transferable skills for working with other microcontrollers and platforms.

## Next Steps
Feel free to contribute to this repository! Add tutorials, share insights, or propose changes to the content. Baremetal programming is a journey of continuous learning and experimentation.

