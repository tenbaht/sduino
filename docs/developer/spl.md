## ST Standard Peripherie Library

The Library an be [downloaded from the ST
website](http://www.st.com/en/embedded-software/stsw-stm8069.html) (free
registration required). A slightly older version is part of the [STM8S_Teach
presentation](https://github.com/VincentYChen/STM8teach/tree/master/code/Project/STM8S_StdPeriph_Examples),

Don't miss the Examples folder within the downloaded
zip file. This and [Lujji's
blog](https://lujji.github.io/blog/bare-metal-programming-stm8/) is the most
useful reference on using this library and programming the STM8 in general.

For use with SDCC the library needs to be patched with Georg's [SPL-SDCC
patches](https://github.com/gicking/STM8-SPL_SDCC_patch):

```bash
unzip en.stsw-stm8069.zip
wget https://raw.githubusercontent.com/gicking/STM8-SPL_SDCC_patch/master/STM8S_StdPeriph_Lib_V2.3.1_sdcc.patch
patch -p0 < STM8S_StdPeriph_Lib_V2.3.1_sdcc.patch
```

Check out the test project and the project template in
`STM8S_StdPeriph_Lib/Project/STM8S_StdPeriph_test` and
`STM8S_StdPeriph_Lib/Project/STM8S_StdPeriph_Template`

SDCC uses .rel as the file extension for its object files.



Now the library can be compiled for the STM8S103 using this Makefile:

```make
CC=sdcc
AR=sdar
CFLAGS=-c -mstm8 -DSTM8S103 -I ../inc --opt-code-size -I.
LDFLAGS=-rc
SOURCES= \
	stm8s_adc1.c	stm8s_awu.c	stm8s_beep.c	stm8s_clk.c \
	stm8s_exti.c	stm8s_flash.c	stm8s_gpio.c	stm8s_i2c.c \
	stm8s_itc.c	stm8s_iwdg.c	stm8s_rst.c	stm8s_spi.c \
	stm8s_tim1.c	stm8s_tim2.c	stm8s_tim4.c	stm8s_uart1.c \
	stm8s_wwdg.c

OBJECTS=$(SOURCES:.c=.o)
OBJECTS_LINK=$(SOURCES:.c=.rel)
EXECUTABLE=stm8s.lib

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
$(AR) $(LDFLAGS) $(EXECUTABLE) $(OBJECTS_LINK)

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *.lib *.rst *.rel *.lst *.ihx *.sym *.asm *.lk *.map
	rm -f $(EXECUTABLE)
```

This library can now be used for linking with blink_spl or uart_spl. The
files stm8s_conf.h and stm8s_it.h are still needed for compilation.


### Optimizing the code

The linker does not remove individual unused functions from an object file,
only complete object files can be skipped.  
**=> for building a library it is better to separate all functions into
individual source files **

See [here](../optimizations/#splitting-files) for details.

A suggestion how to move at least the IRQ vectors away from the libray into
the own source files:
http://richs-words.blogspot.de/2010/09/stm8s-interrupt-handling.html



### Interrupts

Interrupt names are defined in stm8s_itc.h

Declaration of an interrupt function for SDCC:

```c
/* UART1 TX */
void UART1_TX_IRQHandler(void) __interrupt(ITC_IRQ_UART1_TX)
{
}
```

Set the matching IRQ enable flag and globally allow interrupts:

```c
UART1_ITConfig(UART1_IT_TXE, ENABLE);
enableInterrupts();
```

Not sure about the meaning of the ITC priorities. It works even without
them:

```c
ITC_DeInit();
ITC_SetSoftwarePriority(ITC_IRQ_UART1_TX, ITC_PRIORITYLEVEL_2);
```

