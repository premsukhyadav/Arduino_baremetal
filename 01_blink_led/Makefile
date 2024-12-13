all:
	avr-gcc -Os -DF_CPU=16000000UL -mmcu=atmega328p -nostdlib -o blink_led.elf blink_led.c 
	avr-objcopy -O binary blink_led.elf blink_led.bin
	
deploy:
	avrdude -F -V -c arduino -p atmega328p -P COM8 -b 115200 -U flash:w:blink_led.bin

clean:
	rm -rf *.o *.elf *.bin
	