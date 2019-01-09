BASENAME=$(shell basename $$(pwd))
EXECUTABLE=$(BASENAME).ihx

#SDCCBASE=/usr/local
SDCCBASE=/opt/sdcc
BINDIR=$(SDCCBASE)/bin
CC=$(BINDIR)/sdcc
LD=$(BINDIR)/sdld

SDBASE=../../sduino/hardware/sduino/stm8
LIBBASE=$(SDBASE)/STM8S_StdPeriph_Driver
SDUINO=$(SDBASE)/cores/sduino

CPPFLAGS=-DF_CPU=16000000L -DSTM8S103 -Ddouble=float \
	-I. -I$(SDUINO) -I$(SDBASE)/variants/standard \
	-I$(LIBBASE)/inc -I$(LIBBASE)/src \
	-I$(SDCCBASE)/share/sdcc/include/
CFLAGS= -mstm8 --debug
LDFLAGS=-L$(LIBBASE)/lib -L$(SDCCBASE)/share/sdcc/lib/stm8 -lSTM8S103

OBJECTS=pinmode-c.rel pinmode-asm.rel main.rel

.PHONY: all clean flash statistics

#all: $(OBJECTS)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(OBJECTS) : %.rel : %.c
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $^ -o $@

flash: $(EXECUTABLE)
	stm8flash -cstlinkv2 -pstm8s103?3 -w $^

readopt:
	stm8flash -c stlinkv2 -p stm8s103?3 -s opt -r opt.bin


# lists the length of functions and constants defined in the primary source
# file
statistics:
	awk '/ CODE/ {print FILENAME "\t" strtonum("0x"$$4)}' *.sym


#	grep "[0-9] _" $(BASENAME).rst |\
#	awk 'BEGIN {print "length\tfunction\n------\t----------";}{ a=strtonum("0x"$$1);if (name) print a-s "\t" name; s=a; name=$$3}'


clean: 
	rm -f *.lib *.rst *.rel *.lst *.ihx *.sym *.asm *.lk *.map \
		*.cdb *.adb *~ *.bak
	rm -f $(EXECUTABLE)
