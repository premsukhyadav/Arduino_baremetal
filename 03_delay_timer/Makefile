all:
	avr-gcc -Os -DF_CPU=16000000UL -mmcu=atmega328p -nostdlib -o delay_timer.o delay_timer.c
	avr-gcc -o delay_timer.elf delay_timer.o
	avr-objcopy -O binary delay_timer.elf delay_timer.bin
	
deploy:
	avrdude -F -V -c arduino -p atmega328p -P COM8 -b 115200 -U flash:w:delay_timer.bin

clean:
	rm -rf *.o *.elf *.bin
	