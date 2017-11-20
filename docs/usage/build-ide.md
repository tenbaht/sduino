# Building with the Arduino IDE

Sduino now blends in smoothly with the Arduino IDE. Just choose a matching
board type from the list of supported boards at Tools->Board and work as
usual.


- Open the Boards list at Tools->Board:...
- You should find a new entry "STM8S Boards".
- Choose *STM8S103F3 Breakout Board* from the list
- Open the standard Blink example from File->Examples->01. Basics->Blink
- Compile it by clickin 'Verify'
- Or upload it to the connected board by clicking 'Upload'

Done! Your first STM8 based project is up and running!

But keep in mind that it still based on plain C and not 100% Arduino syntax.
It is close enough, that even some of the stock example sketches work right
out of the box, but others will require small modifications.

FIXME: How to access the Sduino adopted example sketches

