
typedef unsigned char	uint8_t;
typedef unsigned short	uint16_t;

/* PORTB*/
#define DDRB_ADDR			(0x24)
#define PORTB_ADDR			(0x25)

/* TIMER/COUNTER 1*/
#define TCCR1A_ADDR			(0x80)
#define TCCR1B_ADDR			(0x81)
#define TCNT1L_ADDR			(0x84)
#define TCNT1H_ADDR			(0x85)
#define TIFR1_ADDR   		(0x36)


__attribute__((naked)) void reset_handler() {
    asm volatile ("rjmp main");
}

// Function to set a bit at a specific address
inline void set_bit(uint16_t addr, uint8_t bit) {
    volatile uint8_t *reg = (volatile uint8_t *)addr; // Create a pointer to the address
    *reg |= (1 << bit);            // Set the specified bit
}

// Function to clear a bit at a specific address
inline void clear_bit(uint16_t addr, uint8_t bit) {
    volatile uint8_t *reg = (volatile uint8_t *)addr; // Create a pointer to the address
    *reg &= ~(1 << bit);           // Clear the specified bit
}

inline void write_reg(uint16_t addr, uint8_t value) {
	volatile uint8_t *reg = (volatile uint8_t *)addr; // Create a pointer to the address
	*reg = value;
}

inline uint8_t read_reg(uint16_t addr) {
	volatile uint8_t *reg = (volatile uint8_t *)addr; // Create a pointer to the address
	return *reg;
}

void delay_1000ms() {
	// reset timer configuration
	write_reg(TCCR1A_ADDR, 0x00);
	write_reg(TCCR1B_ADDR, 0x04);
	
	/* preload the counter with value 0x0BDB */
	write_reg(TCNT1L_ADDR, 0xDB); // preload timer low
	write_reg(TCNT1H_ADDR, 0x0B); // preload timer high
	
	while (!(read_reg(TIFR1_ADDR) & 1));
	
	set_bit(TIFR1_ADDR, 0); // setting the flag actually clear TOV1 flag
}


int main() {
	// set PORTB5 as a output
	set_bit(DDRB_ADDR, 5);

    while(1) 
    {
        // set PORTB5
		set_bit(PORTB_ADDR, 5);
        // wait
		delay_1000ms();
        // unset PORTB5
		clear_bit(PORTB_ADDR, 5);
        // wait some more
		delay_1000ms();
    }
	
	return 0;
}


