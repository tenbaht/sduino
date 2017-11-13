# Installation


## Using the Arduino Board Manager

This is the recommanded way of installation now.

- Open the Settings tab of the Preferences dialog
- Enter
  `https://github.com/tenbaht/sduino/raw/master/package_sduino_stm8_index.json`
  as an Additional Boards Manager URL
- Select *sduino* in the Boards Manager list (Tools->Board:...->Boards Manager)
- Click on Install

Wait for the download to finsh and you are ready to blink:

- Open the Boards list at Tools->Board:...
- You should find a new entry "STM8S Boards".
- Choose *STM8S103F3 Breakout Board* from the list
- Open the standard Blink example from File->Examples->01. Basics->Blink
- Compile it by hitting 'Verify'

Easy, isn't it?

In order to upload the compiled sketch to a connected board you need to
[install your flash tool](../hardware/flashtool/#installation-for-windows).



## Manual install

Only if you prefer a Makefile based build over using the Arduino IDE or if
you would like to help with the development of Sduino you will have to do a
[manual installation](manual-install.md) instead.
