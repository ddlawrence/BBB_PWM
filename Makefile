# CROSSCOMPILE ?= arm-linux-gnueabihf-
CROSSCOMPILE ?= arm-none-eabi-

CFLAGS = -mcpu=cortex-a8 -marm -Wall -O2 -nostdlib -nostartfiles -ffreestanding -fstack-usage -Wstack-usage=8192

all : rts.elf

olib\startup.o : slib\startup.s
	$(CROSSCOMPILE)as slib\startup.s -o olib\startup.o

main.o : main.c
	$(CROSSCOMPILE)gcc $(CFLAGS) -c main.c -o main.o

olib\irq.o : slib\irq.s
	$(CROSSCOMPILE)gcc $(CFLAGS) -c slib\irq.s -o olib\irq.o

olib\gpio.o : slib\gpio.s
	$(CROSSCOMPILE)gcc $(CFLAGS) -c slib\gpio.s -o olib\gpio.o

olib\pwm.o : slib\pwm.s
	$(CROSSCOMPILE)gcc $(CFLAGS) -c slib\pwm.s -o olib\pwm.o

rts.elf : memmap.lds olib\startup.o main.o olib\irq.o olib\gpio.o olib\pwm.o
	$(CROSSCOMPILE)ld -o rts.elf -T memmap.lds olib\startup.o main.o olib\irq.o olib\gpio.o olib\pwm.o
#	$(CROSSCOMPILE)objcopy rts.elf rts.bin -O srec
# srec format for jtag loading (ie binary format with a short header)
# binary format for MMC booting
	$(CROSSCOMPILE)objcopy rts.elf rts.bin -O binary
	$(CROSSCOMPILE)objdump -M reg-names-raw -D rts.elf > rts.lst
#	$(CROSSCOMPILE)objdump -d -S -h -t rts.elf > rts.dmp

clean :
	-@del olib\*.o *.o *.lst *.elf *.bin *.su
