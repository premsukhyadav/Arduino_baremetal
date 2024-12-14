# Custom Delay Implementation Using Arduino Timer1

This example demonstrates how to eliminate the dependency on `<util/delay.h>` by using Timer1 of the ATmega328P to create a precise 1000 ms delay.

---

## Registers Used for Timer1 Configuration

### 1. **TCCR1A (Timer/Counter1 Control Register A)**
   - Configures the behavior of Timer1.
   - In this example, it is set to `0x00` to disable any waveform generation or output compare modes.

### 2. **TCCR1B (Timer/Counter1 Control Register B)**
   - Controls the clock source and prescaler settings for Timer1.
   - Set to `0x04` in this example to use a prescaler of 256.

### 3. **TCNT1H and TCNT1L (Timer/Counter1 High and Low Bytes)**
   - These registers represent the 16-bit Timer1 value.
   - Preloaded with `0x0BDB` (3035 in decimal) so that Timer1 overflows after exactly 1000 ms.

### 4. **TIFR1 (Timer/Counter1 Interrupt Flag Register)**
   - Bit 0 (TOV1) indicates a Timer1 overflow.
   - We use this flag to determine when the delay period has elapsed.

---

## Why Timer1 is Preloaded with 0x0BDB

The timer counts up from the preload value to its maximum value, which is `0xFFFF` for a 16-bit timer. The difference between `0xFFFF` and the preload value determines the duration of the delay. Here's the calculation:

1. **Timer Resolution**: With a prescaler of 256 and a 16 MHz clock, the timer tick frequency is:
   
   \[
   \text{Timer tick frequency} = \frac{16\,\text{MHz}}{256} = 62,500\,\text{Hz} \]
   
   Each tick corresponds to \( \frac{1}{62,500} \approx 16\,\mu s \).

2. **Ticks Required for 1000 ms**:
   
   \[
   \text{Ticks required} = \frac{1\,\text{s}}{16\,\mu s} = 62,500 \]

3. **Preload Value**:
   
   The timer overflows after 65,536 ticks. To achieve 62,500 ticks:
   
   \[
   \text{Preload value} = 65,536 - 62,500 = 3,035 = 0x0BDB \]

Thus, the timer starts counting from `0x0BDB` to ensure a 1000 ms delay.

---

## How the Overflow Flag Works

- **Setting the TOV1 Flag**:
  - When Timer1 overflows, Bit 0 of `TIFR1` is automatically set by the hardware.
  
- **Clearing the TOV1 Flag**:
  - Writing `1` to Bit 0 of `TIFR1` clears the flag. Yes, setting the bit clears the flag!
  - This behavior ensures we can reinitialize the Timer1 overflow logic after each delay.

---

## Inline Functions for Efficiency

We have used the `inline` keyword for the following functions:
- `set_bit()`
- `clear_bit()`
- `write_reg()`
- `read_reg()`

### Benefits of `inline`:
1. **Reduced Code Size**: Inline functions replace function calls with the actual code during compilation, minimizing overhead.
2. **Improved Performance**: Avoiding function call overhead reduces execution time, especially in time-critical embedded applications.

---

## C Code: Delay with Timer1

```c
typedef unsigned char	uint8_t;
typedef unsigned short	uint16_t;

/* PORTB */
#define DDRB_ADDR			(0x24)
#define PORTB_ADDR			(0x25)

/* TIMER/COUNTER 1 */
#define TCCR1A_ADDR			(0x80)
#define TCCR1B_ADDR			(0x81)
#define TCNT1L_ADDR			(0x84)
#define TCNT1H_ADDR			(0x85)
#define TIFR1_ADDR   		(0x36)

__attribute__((naked)) void reset_handler() {
    asm volatile ("rjmp main");
}

inline void set_bit(uint16_t addr, uint8_t bit) {
    volatile uint8_t *reg = (volatile uint8_t *)addr;
    *reg |= (1 << bit);
}

inline void clear_bit(uint16_t addr, uint8_t bit) {
    volatile uint8_t *reg = (volatile uint8_t *)addr;
    *reg &= ~(1 << bit);
}

inline void write_reg(uint16_t addr, uint8_t value) {
	volatile uint8_t *reg = (volatile uint8_t *)addr;
	*reg = value;
}

inline uint8_t read_reg(uint16_t addr) {
	volatile uint8_t *reg = (volatile uint8_t *)addr;
	return *reg;
}

void delay_1000ms() {
	// Reset Timer1 configuration
	write_reg(TCCR1A_ADDR, 0x00);
	write_reg(TCCR1B_ADDR, 0x04);
	
	// Preload Timer1 with 0x0BDB
	write_reg(TCNT1L_ADDR, 0xDB); // Low byte
	write_reg(TCNT1H_ADDR, 0x0B); // High byte
	
	// Wait for overflow flag (TOV1)
	while (!(read_reg(TIFR1_ADDR) & 1));
	
	// Clear overflow flag by setting it
	set_bit(TIFR1_ADDR, 0);
}

int main() {
	// Configure PORTB5 as output
	set_bit(DDRB_ADDR, 5);

    while (1) {
        // Turn on PORTB5
		set_bit(PORTB_ADDR, 5);
        delay_1000ms(); // Wait
        // Turn off PORTB5
		clear_bit(PORTB_ADDR, 5);
        delay_1000ms(); // Wait
    }
	
	return 0;
}
```

---

## Makefile

To compile and upload this code, use the following `Makefile`:

```makefile
all:
	avr-gcc -Os -DF_CPU=16000000UL -mmcu=atmega328p -nostdlib -o delay_timer.o delay_timer.c
	avr-gcc -o delay_timer.elf delay_timer.o
	avr-objcopy -O binary delay_timer.elf delay_timer.bin
	
deploy:
	avrdude -F -V -c arduino -p atmega328p -P COM8 -b 115200 -U flash:w:delay_timer.bin

clean:
	rm -rf *.o *.elf *.bin
```

#### Targets Explained

1. **`all`:**
     ```bash
     avr-gcc -Os -DF_CPU=16000000UL -mmcu=atmega328p -nostdlib -o delay_timer.o delay_timer.c
     avr-gcc -o delay_timer.elf delay_timer.o
     avr-objcopy -O binary delay_timer.elf delay_timer.bin
     ```
   - **Flags Used:**
     - `-Os`: Optimize for size.
     - `-DF_CPU=16000000UL`: Define the clock frequency as 16 MHz.
     - `-mmcu=atmega328p`: Specify the microcontroller model.
     - `-nostdlib`: Exclude standard libraries to minimize size.

2. **`deploy`:**
   - Uploads the generated binary (`delay_timer.bin`) to the microcontroller using `avrdude`.
   - **Command:**
     ```bash
     avrdude -F -V -c arduino -p atmega328p -P COM8 -b 115200 -U flash:w:delay_timer.bin
     ```
   - **Flags Used:**
     - `-F`: Ignore signature check (use cautiously).
     - `-V`: Disable verification after flashing.
     - `-c arduino`: Specify the programmer type.
     - `-p atmega328p`: Specify the microcontroller type.
     - `-P COM8`: Specify the COM port for the connected microcontroller.
     - `-b 115200`: Set the baud rate.
     - `-U flash:w:delay_timer.bin`: Write the binary to the microcontroller's flash memory.

3. **`clean`:**
   - Removes all intermediate and output files (`*.o`, `*.elf`, `*.bin`) to clean up the working directory.
   - **Command:**
     ```bash
     rm -rf *.o *.elf *.bin
     ```

## Disassembly

Disassembling the compiled `.elf` file helps verify the correctness of the generated machine code. Use the following command:

```asm
Disassembly of section .text:

00000000 <reset_handler>:
   0:   0f c0           rjmp    .+30            ; 0x20 <main>

00000002 <delay_1000ms>:
   2:   10 92 80 00     sts     0x0080, r1      ; 0x800080 <__DATA_REGION_ORIGIN__+0x20>
   6:   84 e0           ldi     r24, 0x04       ; 4
   8:   80 93 81 00     sts     0x0081, r24     ; 0x800081 <__DATA_REGION_ORIGIN__+0x21>
   c:   8b ed           ldi     r24, 0xDB       ; 219
   e:   80 93 84 00     sts     0x0084, r24     ; 0x800084 <__DATA_REGION_ORIGIN__+0x24>
  12:   8b e0           ldi     r24, 0x0B       ; 11
  14:   80 93 85 00     sts     0x0085, r24     ; 0x800085 <__DATA_REGION_ORIGIN__+0x25>
  18:   b0 9b           sbis    0x16, 0 ; 22
  1a:   fe cf           rjmp    .-4             ; 0x18 <delay_1000ms+0x16>
  1c:   b0 9a           sbi     0x16, 0 ; 22
  1e:   08 95           ret

00000020 <main>:
  20:   25 9a           sbi     0x04, 5 ; 4
  22:   2d 9a           sbi     0x05, 5 ; 5
  24:   0e 94 01 00     call    0x2     ; 0x2 <delay_1000ms>
  28:   2d 98           cbi     0x05, 5 ; 5
  2a:   0e 94 01 00     call    0x2     ; 0x2 <delay_1000ms>
  2e:   f9 cf           rjmp    .-14            ; 0x22 <main+0x2>

```
### Inline Functions and Code Size Reduction in `delay_timer`

#### Role of Inline Functions
Inline functions reduce function call overhead by replacing the function call with the function body during compilation. In the `delay_timer` program, this optimization helped reduce code size and improve performance. Here's how:

1. **Code Size Reduction:**
   - Functions like `set_bit`, `clear_bit`, `write_reg`, and `read_reg` are declared as `inline`.
   - These are small utility functions that perform single operations on registers.
   - By replacing function calls with the actual code, the compiler eliminates the need for function prologue/epilogue and stack operations.

2. **Visible Impact in Disassembly:**
   - Inline functions do not appear as `call` instructions in the disassembly. Instead, their operations (`sts`, `lds`, `sbi`, etc.) are directly present in the calling function.
   - For example, register manipulations appear directly in the `delay_1000ms` and `main` functions.

3. **Call to `delay_1000ms`:**
   - The `delay_1000ms` function is not marked as `inline`, so the compiler retains it as a standalone function.
   - Calls to `delay_1000ms` are visible in the disassembly:
     ```assembly
     24:   0e 94 01 00     call    0x2     ; 0x2 <delay_1000ms>
     2a:   0e 94 01 00     call    0x2     ; 0x2 <delay_1000ms>
     ```
   - This design ensures modularity and reusability of the delay logic while keeping the main program concise.

#### Balancing Inline and Non-Inline Functions
- **Inline for Utility Functions:** 
  - Small, frequently used functions like `set_bit`, `clear_bit`, etc., are inlined to avoid function call overhead and reduce code size.
- **Non-Inline for Complex Logic:**
  - Functions like `delay_1000ms` remain non-inline to maintain modularity and avoid duplicating complex logic.

#### Key Takeaways
Using inline functions selectively helped reduce the overall program size while keeping critical logic, such as the delay function, modular and reusable. This approach ensures an efficient and maintainable AVR program.


This will produce a detailed assembly listing, showing how the compiler translated the C code into AVR instructions. Check the `main` and `delay_1000ms` functions for accuracy.

---

## Key Points

1. **Timer1 Registers**:
   - `TCCR1A`, `TCCR1B`, `TCNT1H/L`, and `TIFR1` are used to configure and monitor Timer1.
2. **Overflow Flag**:
   - TOV1 in `TIFR1` indicates when Timer1 overflows.
   - Writing `1` to TOV1 clears the flag.
3. **Preload Value**:
   - Timer1 is preloaded with `0x0BDB` to achieve a 1000 ms delay with a prescaler of 256.

