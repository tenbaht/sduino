BASENAME=$(shell basename $$(pwd))
EXECUTABLE=$(BASENAME).ihx

#SDCCBASE=/usr/local
SDCCBASE=/opt/sdcc
BINDIR=$(SDCCBASE)/bin
CC=$(BINDIR)/sdcc
LD=$(BINDIR)/sdld

LIBBASE=../../sduino/stm8/STM8S_StdPeriph_Driver
SDUINO=../../sduino/stm8/cores/sduino
PINS=../../sduino/stm8/variants/standard

CFLAGS= --debug -mstm8 -DF_CPU=2000000L -DSTM8S103 \
	-I. -I$(SDUINO) -I$(LIBBASE)/inc -I$(PINS) -I/usr/share/sdcc/include/

LDFLAGS=-L$(LIBBASE)/src -L$(SDCCBASE)/share/sdcc/lib/stm8 -lstm8s

OBJECTS=$(BASENAME).rel HardwareSerial.rel
#SDOBJECTS=HardwareSerial.rel

.PHONY: all clean flash

#all: $(OBJECTS)

$(EXECUTABLE): $(OBJECTS) $(SDOBJECTS)
#test.rel wiring_digital.rel
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(OBJECTS) : %.rel : %.c
	$(CC) -c $(CFLAGS) $^ -o $@

$(SDOBJECTS) : %.rel : $(SDUINO)/%.c
	$(CC) -c $(CFLAGS) $^ -o $@

flash: $(EXECUTABLE)
	stm8flash -cstlinkv2 -pstm8s103?3 -w $^

# Re-build HardwareSerial.c from the little parts in the core directory
HardwareSerial.c:
	grep -hv '#include "HardwareSerial.c.h"' \
		$(SDUINO)/HardwareSerial.c.h \
		$(SDUINO)/HardwareSerial.c-*.c > $@

clean: 
	rm -f *.lib *.rst *.rel *.lst *.ihx *.sym *.asm *.lk *.map \
		*.cdb *.adb *~ *.bak
	rm -f $(EXECUTABLE)
