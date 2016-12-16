EXECUTABLE=alles.ihx

#SDCCBASE=/usr/local
SDCCBASE=/opt/sdcc
BINDIR=$(SDCCBASE)/bin
CC=$(BINDIR)/sdcc
LD=$(BINDIR)/sdld

CFLAGS= --debug -mstm8 -DF_CPU=16000000L -DSTM8S103 \
	-I/usr/share/sdcc/include/ -I$(LIBBASE)/inc -I.

LIBBASE=../STM8S_StdPeriph_Driver
LDFLAGS=-L$(LIBBASE)/src -L/opt/sdcc/share/sdcc/lib/stm8 -lstm8s

OBJECTS=main.rel wiring.rel wiring_digital.rel \
	wiring_shift.rel wiring_pulse.rel \
	HardwareSerial.rel \
	sketch.rel

.PHONY: all clean flash

#all: $(OBJECTS)

$(EXECUTABLE): $(OBJECTS)
#test.rel wiring_digital.rel
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@
	
$(OBJECTS) : %.rel : %.c
	$(CC) -c $(CFLAGS) $^

.c.o:
	$(CC) -c $(CFLAGS) $^ -o $@

.c.rel:
	$(CC) -c $(CFLAGS) $^

flash: $(EXECUTABLE)
	stm8flash -cstlinkv2 -pstm8s103?3 -w $^


clean: 
	rm -f *.lib *.rst *.rel *.lst *.ihx *.sym *.asm *.lk *.map \
		*.cdb *.adb *~ *.bak
	rm -f $(EXECUTABLE)
