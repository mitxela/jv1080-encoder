.PHONY: clean flash disasm

sc.bin: makefile sc.c
	avr-gcc -mmcu=attiny13a -Wall -Os -g -o sc.bin sc.c

sc.hex: sc.bin
	avr-objcopy -j .text -j .data -O ihex sc.bin sc.hex

clean:
	rm -f sc.hex sc.bin

flash: sc.hex
	avrdude -c usbasp -p t13 -U flash:w:sc.hex:i -B50

disasm:	sc.bin
	avr-objdump -d sc.bin
