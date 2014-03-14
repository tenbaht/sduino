SDCC=sdcc
SDLD=sdld
OBJECTS=blinky.bin uart.bin sp_test.bin
HEX2BIN=hex2bin

.PHONY: all clean flash

all: $(OBJECTS)

clean:
	rm -f $(OBJECTS)

flash: $(OBJECT).bin
	stm8flash -cstlink -pstm8l150 -w $(OBJECT).bin

%.ihx: %.c
	$(SDCC) -lstm8 -mstm8 --out-fmt-ihx $(CFLAGS) $(LDFLAGS) $<

%.bin: %.ihx
	$(HEX2BIN) -p 00 $<
