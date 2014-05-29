avr-gcc -mmcu=attiny84 -Wall -Os -o tone.elf tone.c
avr-objcopy -j .text -j .data -O ihex tone.elf tone.hex
sudo avrdude -p t84 -c usbtiny -U flash:w:tone.hex
