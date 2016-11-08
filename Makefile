SDCC=sdcc
SDLD=sdld
OBJECT=blink
OBJECTS=blink_spl.rel stm8s_gpio.rel

CFLAGS=-DSTM8S103 -I. -I../STM8S_StdPeriph_Lib/Libraries/STM8S_StdPeriph_Driver/inc/


.PHONY: all clean flash

all: $(OBJECT).ihx

clean:
	rm -f *.asm *.ihx *.rel *.sym *.map *.cdb *.lk *.lst *.rst *~

flash: $(OBJECT).ihx
	stm8flash -cstlink -pstm8l150 -w $(OBJECT).ihx

$(OBJECT).ihx: $(OBJECTS)
	$(SDCC) -lstm8 -mstm8 --out-fmt-ihx $(LDFLAGS) $^ -o $@

%.rel: %.c
	$(SDCC) -lstm8 -mstm8 --out-fmt-ihx $(CFLAGS) $(LDFLAGS) -c $<
