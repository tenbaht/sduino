SDCC=sdcc
SDLD=sdld
OBJECTS=blink_spl.ihx stm8s_gpio.ihx

CFLAGS=-DSTM8S103 -I. -I../STM8S_StdPeriph_Lib/Libraries/STM8S_StdPeriph_Driver/inc/ -c


.PHONY: all clean flash

all: $(OBJECTS)

clean:
	rm -f $(OBJECTS)

flash: $(OBJECT).ihx
	stm8flash -cstlink -pstm8l150 -w $(OBJECT).ihx

%.ihx: %.c
	$(SDCC) -lstm8 -mstm8 --out-fmt-ihx $(CFLAGS) $(LDFLAGS) $<
