BOARD_TAG	= stm8sblue
ARDUINO_DIR	= ../../sduino
ARDMK_VENDOR	= sduino
ARCHITECTURE	= stm8
CPPFLAGS	= -DSTM8S103 -I ../../STM8S_StdPeriph_Driver/inc
#CFLAGS		= -I ../../STM8S_StdPeriph_Driver/inc
LD_FLAGS	= --out-fmt-elf

MCU_FLAG_NAME	= m
#MMD = "-Wp-MMD abc"

OVERRIDE_EXECUTABLES=yes
SDCC_PATH=/opt/sdcc/bin
    CC      = $(SDCC_PATH)/sdcc
    AS      = $(SDCC_PATH)/sdasstm8
    AR      = $(SDCC_PATH)/sdar

#ALTERNATE_CORE    = sduino
#ARDUINO_VAR_PATH  = ../../sduino
#ARDUINO_CORE_PATH = ../../sduino



include Arduino.mk
