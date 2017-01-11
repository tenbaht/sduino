BASENAME=$(shell basename $$(pwd))
EXECUTABLE=$(BASENAME).ihx

#SDCCBASE=/usr/local
SDCCBASE=/opt/sdcc
BINDIR=$(SDCCBASE)/bin
CC=$(BINDIR)/sdcc
LD=$(BINDIR)/sdld

LIBBASE=../../STM8S_StdPeriph_Driver
SDUINO=../../sduino

CFLAGS= --debug -mstm8 -DF_CPU=16000000L -DSTM8S103 \
	-I. -I$(SDUINO) -I$(LIBBASE)/inc -I/usr/share/sdcc/include/
# -DSUPPORT_ALTERNATE_MAPPINGS 

LDFLAGS=-L$(LIBBASE)/src -L/opt/sdcc/share/sdcc/lib/stm8 -lstm8s103

OBJECTS=$(BASENAME).rel
SDOBJECTS=main.rel wiring.rel wiring_digital.rel SPI.rel \
	HardwareSerial.rel Print.rel

.PHONY: all clean flash statistics

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

readopt:
	stm8flash -c stlinkv2 -p stm8s103?3 -s opt -r opt.bin


# lists the length of functions and constants defined in the primary source
# file
statistics: $(EXECUTABLE)
	grep "[0-9] _" $(BASENAME).rst |\
	awk 'BEGIN {print "length\tfunction\n------\t----------";}{ a=strtonum("0x"$$1);if (name) print a-s "\t" name; s=a; name=$$3}'


clean: 
	rm -f *.lib *.rst *.rel *.lst *.ihx *.sym *.asm *.lk *.map \
		*.cdb *.adb *~ *.bak
	rm -f $(EXECUTABLE)
