BASENAME=$(shell basename $$(pwd))
EXECUTABLE=$(BASENAME).ihx

#SDCCBASE=/usr/local
SDCCBASE=/opt/sdcc
BINDIR=$(SDCCBASE)/bin
CC=$(BINDIR)/sdcc
LD=$(BINDIR)/sdld

LIBBASE=../../STM8S_StdPeriph_Driver
SDUINO=../../sduino

CFLAGS= --debug -mstm8 -DF_CPU=2000000L -DSTM8S103 \
	-I. -I$(SDUINO) -I$(LIBBASE)/inc -I/usr/share/sdcc/include/

LDFLAGS=-L$(LIBBASE)/src -L/opt/sdcc/share/sdcc/lib/stm8 -lstm8s

OBJECTS=$(BASENAME).rel
SDOBJECTS=main.rel wiring.rel wiring_digital.rel HardwareSerial.rel

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


clean: 
	rm -f *.lib *.rst *.rel *.lst *.ihx *.sym *.asm *.lk *.map \
		*.cdb *.adb *~ *.bak
	rm -f $(EXECUTABLE)
