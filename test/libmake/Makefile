BOARD_TAG	= stm8sblue
ARDUINO_DIR	= ../../sduino
ARDMK_VENDOR	= sduino
ARCHITECTURE	= stm8
CPPFLAGS	= -DSTM8S103 -I ../../STM8S_StdPeriph_Driver/inc
#CFLAGS		= -I ../../STM8S_StdPeriph_Driver/inc
#LDFLAGS		= --out-fmt-elf 
LDFLAGS		= -L ../../STM8S_StdPeriph_Driver/src

MCU_FLAG_NAME	= m
#MMD = "-Wp-MMD abc"

OVERRIDE_EXECUTABLES=yes
SDCC_PATH=/opt/sdcc/bin
    CC      = $(SDCC_PATH)/sdcc
    AS      = $(SDCC_PATH)/sdasstm8
    AR      = $(SDCC_PATH)/sdar
    SIZE    = /usr/bin/size

#ALTERNATE_CORE    = sduino
#ARDUINO_VAR_PATH  = ../../sduino
#ARDUINO_CORE_PATH = ../../sduino


all:

include Arduino.mk

TARGET_HEX = $(OBJDIR)/$(TARGET).ihx

all: 		$(TARGET_HEX)

upload:		$(TARGET_HEX) verify_size
		$(MAKE) reset
		$(MAKE) do_upload

