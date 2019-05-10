BASENAME=$(shell basename $$(pwd))
EXECUTEABLE=$(BASENAME).ihx

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
CFLAGS= -mstm8 #--debug
LDFLAGS=-L$(LIBBASE)/lib -L$(SDCCBASE)/share/sdcc/lib/stm8 -lSTM8S103

SRCS=pinmode-c.c pinmode-asm.c main.c

BUILDDIR=build
OBJECTS=$(SRCS:%.c=$(BUILDDIR)/%.rel)

.PHONY: all clean flash statistics sim

all: $(EXECUTEABLE) statistics sim

$(EXECUTEABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(OBJECTS) : $(BUILDDIR)/%.rel : %.c
	mkdir -p $(BUILDDIR)
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $^ -o $@

flash: $(EXECUTABLE)
	stm8flash -cstlinkv2 -pstm8s103?3 -w $^

readopt:
	stm8flash -c stlinkv2 -p stm8s103?3 -s opt -r opt.bin


# lists the length of functions and constants defined in the primary source
# file
statistics: $(OBJECTS)
	cd $(BUILDDIR); awk '/ CODE/ {print FILENAME "\t" strtonum("0x"$$4)}' *.sym

sim: $(EXECUTEABLE)
	@. ./sim

#	@. ./sim | grep "0x0001 00 00 00 00 00 00"
#	. ./sim | grep "0x0001 00 00 00 00 00 00" || echo "fail"


#	grep "[0-9] _" $(BASENAME).rst |\
#	awk 'BEGIN {print "length\tfunction\n------\t----------";}{ a=strtonum("0x"$$1);if (name) print a-s "\t" name; s=a; name=$$3}'


clean: 
	rm -rf *.lib *.rst *.rel *.lst *.ihx *.sym *.asm *.lk *.map \
		*.cdb *.adb *~ *.bak build
	rm -f $(EXECUTABLE)
