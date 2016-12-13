SDCC=sdcc
SDLD=sdld
OBJECTS=blinky.ihx uart.ihx #sp_test.ihx

.PHONY: all clean flash

all: $(OBJECTS)

clean:
	rm -f $(OBJECTS)

flash: $(OBJECT).ihx
	stm8flash -cstlink -pstm8l150 -w $(OBJECT).ihx

%.ihx: %.c
	$(SDCC) -lstm8 -mstm8 --out-fmt-ihx $(CFLAGS) $(LDFLAGS) $<
