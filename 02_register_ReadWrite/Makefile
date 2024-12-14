all:
	avr-gcc -Os -DF_CPU=16000000UL -mmcu=atmega328p -nostdlib -o reg_readwrite.elf reg_readwrite.c 
	avr-objcopy -O binary reg_readwrite.elf reg_readwrite.bin
	
deploy:
	avrdude -F -V -c arduino -p atmega328p -P COM8 -b 115200 -U flash:w:reg_readwrite.bin

clean:
	rm -rf *.o *.elf *.bin

	