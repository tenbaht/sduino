BASENAME=$(shell basename $$(pwd))
EXECUTABLE=$(BASENAME).ihx

#SDCCBASE=/usr/local
SDCCBASE=/opt/sdcc
BINDIR=$(SDCCBASE)/bin
CC=$(BINDIR)/sdcc
LD=$(BINDIR)/sdld
AS=$(BINDIR)/sdasstm8

LIBBASE=../../sduino/stm8/STM8S_StdPeriph_Driver
SDUINO=../../sduino

CFLAGS= --debug -mstm8 -DF_CPU=16000000L -DSTM8S103 -DSUPPORT_ALTERNATE_MAPPINGS \
	-I. -I$(SDUINO) -I$(LIBBASE)/inc -I/usr/share/sdcc/include/

LDFLAGS=-L$(LIBBASE)/lib -L$(SDCCBASE)/share/sdcc/lib/stm8 -lSTM8S103
ASFLAGS=

OBJECTS=$(BASENAME).rel
ASMOBJECTS=printhex.rel
SDOBJECTS=
#	main.rel wiring.rel wiring_digital.rel wiring_analog.rel \
#	HardwareSerial.rel Print.rel

.PHONY: all clean flash

#all: $(OBJECTS)

$(EXECUTABLE): $(OBJECTS) $(SDOBJECTS) $(ASMOBJECTS)
#test.rel wiring_digital.rel
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(OBJECTS) : %.rel : %.c
	$(CC) -c $(CFLAGS) $^ -o $@

$(SDOBJECTS) : %.rel : $(SDUINO)/%.c
	$(CC) -c $(CFLAGS) $^ -o $@

$(ASMOBJECTS) : %.rel : %.s
	$(AS) -o -l -s $(ASFLAGS) $^

flash: $(EXECUTABLE)
	stm8flash -cstlinkv2 -pstm8s103?3 -w $^

readopt:
	stm8flash -c stlinkv2 -p stm8s103?3 -s opt -r opt.bin


clean: 
	rm -f *.lib *.rst *.rel *.lst *.ihx *.sym *.asm *.lk *.map \
		*.cdb *.adb *~ *.bak
	rm -f $(EXECUTABLE)
