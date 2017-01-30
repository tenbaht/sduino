## ST Standard Library

Can be [downloaded from the ST website]
(http://www.st.com/en/embedded-software/stsw-stm8069.html)
(free registration required). Don't miss the Examples folder within the
downloaded zip file. This is the most useful reference on using this library
and programming the STM8 in general.

For use with SDCC the library needs to be patched:

	git clone https://github.com/g-gabber/STM8S_StdPeriph_Driver.git
	git clone https://github.com/gicking/SPL_2.2.0_SDCC_patch.git
	cp ../STM8S_SPL_2.2.0/Libraries/STM8S_StdPeriph_Driver/inc/stm8s.h .
	patch -p1 < ../SPL_2.2.0_SDCC_patch/STM8_SPL_v2.2.0_SDCC.patch
	cp -av  ../STM8S_StdPeriph_Lib/Project/STM8S_StdPeriph_Template/stm8s_conf.h .
	cp -av  ../STM8S_StdPeriph_Lib/Project/STM8S_StdPeriph_Template/stm8s_it.h .

SDCC uses .rel as the file extension for its object files.

Additional patch required for stm8s_itc.c:

```diff
--- stm8s_itc.c~	2014-10-21 17:32:20.000000000 +0200
+++ stm8s_itc.c	2016-12-11 21:56:41.786048494 +0100
@@ -55,9 +55,12 @@
   return; /* Ignore compiler warning, the returned value is in A register */
 #elif defined _RAISONANCE_ /* _RAISONANCE_ */
   return _getCC_();
-#else /* _IAR_ */
+#elif defined _IAR_ /* _IAR_ */
   asm("push cc");
   asm("pop a"); /* Ignore compiler warning, the returned value is in A register */
+#else /* _SDCC_ */
+  __asm__("push cc");
+  __asm__("pop a"); /* Ignore compiler warning, the returned value is in A register */
 #endif /* _COSMIC_*/
 }
```



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

The linker does not remove individual unused functions from an object file,
only complete object files can be skipped.  
**=> for building a library it is better to separate all functions into
individual source files **

The SPL folder in this archive contains a script `doit` to separate the
functions before compilation.
FIXME: description needed

Erklärung wie zumindest die Interrupt-Vektoren in die eigene Datei kommen
können:
http://richs-words.blogspot.de/2010/09/stm8s-interrupt-handling.html



### Interrupts

Namen definiert in stm8s_itc.h
Interrupt-Routine definieren:

```c
/* UART1 TX */
void UART1_TX_IRQHandler(void) __interrupt(ITC_IRQ_UART1_TX)
{
}
```

Jetzt muss noch das passende IRQ-Enable-Flag gesetzt werden und Interrupt
generell freigegeben werden, also hier:

```c
UART1_ITConfig(UART1_IT_TXE, ENABLE);
enableInterrupts();
```

Unklar ist, was die ITC-Prioritäten bewirken. Es geht jedenfalls auch ohne:

```c
ITC_DeInit();
ITC_SetSoftwarePriority(ITC_IRQ_UART1_TX, ITC_PRIORITYLEVEL_2);
```

