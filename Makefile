SDCC=sdcc
SDLD=sdld
OBJECTS=blinky.bin uart.bin
HEX2BIN=hex2bin

.PHONY: all clean flash

all: $(OBJECTS)

clean:
	rm -f $(OBJECTS)

flash: $(OBJECT).bin
	stm8flash -cstlink -pstm8l150 -wf $(OBJECT).bin

%.ihx: %.c
	$(SDCC) -mstm8 --out-fmt-ihx $(CFLAGS) $(LDFLAGS) $<

%.bin: %.ihx
	$(HEX2BIN) -p 00 $<
