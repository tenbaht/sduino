BASENAME=$(shell basename $$(pwd))
EXECUTABLE=$(BASENAME).ihx

SDCCBASE=/opt/sdcc
BINDIR=$(SDCCBASE)/bin
LIBDIR=$(SDCCBASE)/share/sdcc/lib
INCDIR=$(SDCCBASE)/share/include
CC=$(BINDIR)/sdcc
LD=$(BINDIR)/sdld

SPLBASE=../../../STM8S_StdPeriph_Driver
SDUINO=../../../sduino

CFLAGS= --debug -mstm8 -DF_CPU=16000000L -DSTM8S103 \
	-I. -I$(SDUINO) -I$(SPLBASE)/inc -I$(INCDIR)

LDFLAGS=-L$(SPLBASE)/src -L$(LIBDIR)/stm8 -lstm8s103

OBJECTS=$(BASENAME).rel
SDOBJECTS=main.rel wiring.rel wiring_digital.rel \
	HardwareSerial.rel Print.rel
#	wiring_analog.rel

.PHONY: all clean flash

#all: $(OBJECTS)

$(EXECUTABLE): $(OBJECTS) $(SDOBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(OBJECTS) : %.rel : %.c
	$(CC) -c $(CFLAGS) $^ -o $@

$(SDOBJECTS) : %.rel : $(SDUINO)/%.c
	$(CC) -c $(CFLAGS) $^ -o $@

flash: $(EXECUTABLE)
	stm8flash -cstlinkv2 -pstm8s103?3 -w $^

readopt:
	stm8flash -c stlinkv2 -p stm8s103?3 -s opt -r opt.bin


clean: 
	rm -f *.lib *.rst *.rel *.lst *.ihx *.sym *.asm *.lk *.map \
		*.cdb *.adb *~ *.bak
	rm -f $(EXECUTABLE)
