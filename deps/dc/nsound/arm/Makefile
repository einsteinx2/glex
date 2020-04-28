all: nstream.drv

aica_fw.h: nstream.drv
	$(KOS_BASE)/utils/bin2c/bin2c nstream.drv aica_fw.h.tmp
	echo 'unsigned char aica_fw[] = {' > aica_fw.h
	cat aica_fw.h.tmp >> aica_fw.h
	echo '};' >> aica_fw.h
	-rm aica_fw.h.tmp

nstream.drv: prog.elf
	$(DC_ARM_OBJCOPY) -O binary prog.elf nstream.drv

prog.elf: crt0.o main.o aica.o
	$(DC_ARM_CC) -Wl,-Ttext,0x00000000,-Map,prog.map,-N -nostartfiles -nostdlib -e reset -o prog.elf crt0.o main.o aica.o -lgcc

%.o: %.c
	$(DC_ARM_CC) $(DC_ARM_CFLAGS) $(DC_ARM_INCS) -c $< -o $@

%.o: %.s
	$(DC_ARM_AS) $(DC_ARM_AFLAGS) $< -o $@

clean:
	-rm -f *.o *.srec *.elf 1ST_READ.BIN prog.bin *.bck prog.map


