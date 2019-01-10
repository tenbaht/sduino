original version (276 bytes)
      0080A0                         47 _pinMode1:
      0081B4                        268 _setup:
      0081BE                        282 _loop:
after first commit (176 bytes):
      0080A0                         48 _pinMode1:
      0081B4                        269 _pinMode_asm:
      008264                        410 _setup:
      00826E                        424 _loop:

Reihenfolgen:
INPUT		|-CR2, -CR1, -DDR
INPUT_PULLUP	|-CR2, -DDR, +CR1
OUTPUT_FAST	|+CR1, +DDR, +CR2
OUTPUT_OD_FAST	|-CR1, +DDR, +CR2
OUTPUT_OD	|-CR1, -CR2, +DDR
OUTPUT		|+CR1, -CR2, +DDR

mode	CR2


sstm8 -tS003 -S uart=1,port=10000 mini.ihx


CR1: pullup bzw. open-drain

CR2: in: INT, out: fast
- input: als erstes reset
- output: als letztes setzen (nach DDR)


Schritt 1: CR2 (in: INT, out: fast)
- set: OUTPUT_FAST, OUTPUT_OD_FAST
- reset: sonst


## Simulator

-i m: info memory
