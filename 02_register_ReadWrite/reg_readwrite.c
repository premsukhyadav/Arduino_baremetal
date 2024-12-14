//#include <avr/io.h>
#include <util/delay.h>

#define DDRB_ADDR		(0x24)
#define PORTB_ADDR		(0x25)

__attribute__((naked)) void reset_handler() {
    asm volatile ("rjmp main");
}

void set_bit(uint16_t addr, uint8_t bit) {
    uint8_t *reg = (uint8_t *)addr; // Create a pointer to the address
    *reg |= (1 << bit);            // Set the specified bit
}

// Function to clear a bit at a specific address
void clear_bit(uint16_t addr, uint8_t bit) {
    uint8_t *reg = (uint8_t *)addr; // Create a pointer to the address
    *reg &= ~(1 << bit);           // Clear the specified bit
}

int main() {
	
	// set PORTB5 as a output
	set_bit(DDRB_ADDR, 5);

    while(1) 
    {
        // set PORTB5
		set_bit(PORTB_ADDR, 5);

        // wait
        _delay_ms(1000);

        // unset PORTB5
		clear_bit(PORTB_ADDR, 5);

        // wait some more
        _delay_ms(1000);
    }
}




