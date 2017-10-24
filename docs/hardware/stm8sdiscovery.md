# STM8S Discovery

A widespread evaluation board made by ST. All CPU pins are easily accessible
on 2x6 pin headers. It costs only $9 and includes a ST-Link/v1 flash
programmer on board.

It should work now, but the support is not thoroughly tested. At least
Blink.c is known to work already.

STM8S105C6T6 microcontroller, 32 KB Flash, 2 KB RAM, 1 KB EEPROM 

  - LED on PD0, active low.
  - Touch button on PC1, PC2, PC3
  - external crystal 16MHz on PA1/PA2

The touch pins PC1 and PC3 (Arduino-Pin 24 and 26) are not connected to the
pin header on the board. To use them as I/O-pins you need to modify some
solder bridges (See STM8S-Discovery user manual).


## SWIM connector

Pinout of CN7 SWIM connector

Pin	| Signal
---:	| ------
1	| Vdd (3.3V)
2	| SWIM
3	| GND
4	| NRST


## Usage with Linux and stm8flash

The discovery board implements two USB devices: A ST-LINK/V1 interface and a
(not very useful) mass storage device with some links to the ST website. The
implementation of this mass storage device is very buggy and renders the
hole board useless. You need to make modprobe to ignore it by adding this
line to /etc/modprobe.conf or by adding a file
/etc/modprobe.d/stdiscovery.conf with this line:

```bash
	options usb-storage quirks=0483:3744:i
```

Unplug the Discovery board and unload the usb mass storage driver with

```bash
	modprobe -r uas usb_storage
```

If you can't unload the usb_storage drive because it is in use with other
devices you can temporary trigger the same effect by this line:

```bash
	echo "0483:3744:i" >/sys/module/usb_storage/parameters/quirks
```

The second step is to add a new udev rule in order to access the USB port.
Save this as root in in `/etc/udev/rules.d/99-stlink.rules`:

```bash
	# ST-Link/V2 programming adapter

	# ST-Link V1, if using a STM8S discovery board
	# important: It needs a special entry in /etc/modprobe.d
	ATTR{idVendor}=="0483", ATTR{idProduct}=="3744", MODE="0666", GROUP="plugdev"

	# ST-Link/V2, the china adapter with the green plastic housing
	ATTR{idVendor}=="0483", ATTR{idProduct}=="3748", MODE="0666", GROUP="plugdev"
```

Finally, it is time to (re-) connect the board. Now dmesg should show that
the mass storage device of the Discovery board is ignored:

```bash
	[  815.228928] usbcore: deregistering interface driver uas
	[  815.229201] usbcore: deregistering interface driver usb-storage
	[  823.001086] usb 1-3: new full-speed USB device number 7 using xhci_hcd
	[  823.374719] usb 1-3: New USB device found, idVendor=0483, idProduct=3744
	[  823.374724] usb 1-3: New USB device strings: Mfr=1, Product=2, SerialNumber=3
	[  823.374726] usb 1-3: Product: STM32 STLink
	[  823.374728] usb 1-3: Manufacturer: STMicroelectronics
	[  823.374730] usb 1-3: SerialNumber: W\xffffffc3\xffffffbf\xffffffbfp\x06reWW7\x18\x12g
	[  823.746878] usb-storage 1-3:1.0: USB Mass Storage device detected
	[  823.748299] usb-storage 1-3:1.0: device ignored
	[  823.748453] usbcore: registered new interface driver usb-storage
	[  823.771094] usbcore: registered new interface driver uas
```

Let's read the preinstalled firmware of the STM8S105:

```bash
	stm8flash -cstlink "-pstm8s105?6" -r flash.img
```


## Further reading

[AN3332](http://www.st.com/resource/en/application_note/cd00296680.pdf):
Generating PWM signals using STM8S-DISCOVERY 

[AN3259](http://www.st.com/resource/en/application_note/cd00282842.pdf):
RS232 communications with a terminal using the STM8S-DISCOVERY

