#include "stm8s.h"                                         // (1)

main()                                                     // (2)
{
  GPIO_DeInit(GPIOD);                                      // (3)
  GPIO_Init(GPIOD, GPIO_PIN_0,GPIO_MODE_OUT_PP_LOW_SLOW);  // (4)
	
  while (1)                                                // (5)
  {
    u16 i;                                                 // (6)
	
    for(i = 0; i < 60000; i++)                             // (7)
    {
      nop();                                               // (8)
    }
		
    GPIO_WriteReverse(GPIOD, GPIO_PIN_0);                  // (9)
  }                                                        // (10)
}                                                          // (11)

