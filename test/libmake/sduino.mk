ARDUINO_DIR	= ../../sduino
ARDUINO_SKETCHBOOK	= /tmp	# temporarly, to prevent usage of the real libs
ARDMK_VENDOR	= sduino
ARCHITECTURE	= stm8
CPPFLAGS	= -DSTM8S103 -I../../STM8S_StdPeriph_Driver/inc -I/opt/sdcc/share/sdcc/include/
#CFLAGS		= -I ../../STM8S_StdPeriph_Driver/inc
#LDFLAGS		= --out-fmt-elf 
LDFLAGS		= -L ../../STM8S_StdPeriph_Driver/src -L/opt/sdcc/share/sdcc/lib/stm8


MCU_FLAG_NAME	= m

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


STM8FLASH = stm8flash
STM8FLASH_COM_OPTS =
STM8FLASH_ARD_OPTS = -c$(AVRDUDE_ARD_PROGRAMMER) -p$(AVRDUDE_MCU)
STM8FLASH_UPLOAD_HEX = -s flash -w $(TARGET_HEX)
STM8FLASH_UPLOAD_EEP = -s eeprom -w $(TARGET_EEP)

upload: $(TARGET_HEX) verify_size
	$(STM8FLASH) $(STM8FLASH_COM_OPTS) $(STM8FLASH_ARD_OPTS) \
		$(STM8FLASH_UPLOAD_HEX)
