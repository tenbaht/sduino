BASENAME=$(shell basename $$(pwd))
EXECUTABLE=$(BASENAME).ihx

#SDCCBASE=/usr/local
SDCCBASE=/opt/sdcc
BINDIR=$(SDCCBASE)/bin
CC=$(BINDIR)/sdcc
LD=$(BINDIR)/sdld

LIBBASE=../../STM8S_StdPeriph_Driver
SDUINO=../../sduino

CFLAGS= --debug -mstm8 -DF_CPU=16000000L -DSTM8S103 -DARDUINO=180 \
	-I. -I$(SDUINO) -I$(SDUINO)/libraries/I2C -I$(LIBBASE)/inc \
	-I/usr/share/sdcc/include/
# -DSUPPORT_ALTERNATE_MAPPINGS 

LDFLAGS=-L$(LIBBASE)/src -L/opt/sdcc/share/sdcc/lib/stm8 -lstm8s103

OBJECTS=$(BASENAME).rel ssd1306.rel
#SDLIBS=I2C
SDOBJECTS=main.rel wiring.rel wiring_digital.rel
SDLIBOBJECTS=I2C.rel

.PHONY: all clean flash

#all: $(OBJECTS)

$(EXECUTABLE): $(OBJECTS) $(SDOBJECTS) $(SDLIBOBJECTS)
#test.rel wiring_digital.rel
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(OBJECTS) : %.rel : %.c
	$(CC) -c $(CFLAGS) $^ -o $@

$(SDOBJECTS) : %.rel : $(SDUINO)/%.c
	$(CC) -c $(CFLAGS) $^ -o $@

$(SDLIBOBJECTS) : %.rel : $(SDUINO)/libraries/I2C/%.c
	$(CC) -c $(CFLAGS) $^ -o $@

flash: $(EXECUTABLE)
	stm8flash -cstlinkv2 -pstm8s103?3 -w $^

readopt:
	stm8flash -c stlinkv2 -p stm8s103?3 -s opt -r opt.bin


clean: 
	rm -f *.lib *.rst *.rel *.lst *.ihx *.sym *.asm *.lk *.map \
		*.cdb *.adb *~ *.bak
	rm -f $(EXECUTABLE)
