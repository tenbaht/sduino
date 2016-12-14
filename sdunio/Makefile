EXECUTABLE=alles.ihx

CC=/usr/bin/sdcc
CFLAGS= -mstm8 -DF_CPU=16000000L -DSTM8S103 --no-peep \
	-I/usr/share/sdcc/include/ -I$(LIBBASE)/inc -I.

LIBBASE=../STM8S_StdPeriph_Driver
LIBFILES=$(LIBBASE)/src/stm8s.lib
LDFLAGS=-L../STM8S_StdPeriph_Driver/src -lstm8s

OBJECTS=wiring.rel wiring_digital.rel test.rel

.PHONY: all clean

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

clean: 
	rm -f *.lib *.rst *.rel *.lst *.ihx *.sym *.asm *.lk *.map *.cdb \
		*~ *.bak
	rm -f $(EXECUTABLE)
