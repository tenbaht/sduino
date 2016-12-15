SDCCBASE=/opt/sdcc
BINDIR=$(SDCCBASE)/bin
SDCC=$(BINDIR)/sdcc
SDLD=$(BINDIR)/sdld

OBJECT=uart
OBJECTS=uart_spl.rel

LIBDIR=../../STM8S_StdPeriph_Driver/src
#LIBFILES=$(LIBDIR)/stm8s_gpio.rel
LIBFILES=$(LIBDIR)/stm8s.lib

CFLAGS=-DSTM8S103 -I. -I../../STM8S_StdPeriph_Driver/inc --debug


.PHONY: all clean flash

all: $(OBJECT).ihx

clean:
	rm -f *.asm *.ihx *.rel *.sym *.map *.cdb *.lk *.lst *.rst *~

flash: $(OBJECT).ihx
	stm8flash -cstlinkv2 -pstm8s103?3 -w $(OBJECT).ihx

$(OBJECT).ihx: $(OBJECTS) $(LIBFILES)
	$(SDCC) -lstm8 -mstm8 --out-fmt-ihx $(LDFLAGS) $^ -o $@

%.rel: %.c
	$(SDCC) -lstm8 -mstm8 --out-fmt-ihx $(CFLAGS) $(LDFLAGS) -c $<
