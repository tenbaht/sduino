#include "stm8s.h"                                         // (1)

main()                                                     // (2)
{
  GPIO_DeInit(GPIOB);                                      // (3)
  GPIO_Init(GPIOB, GPIO_PIN_5,GPIO_MODE_OUT_PP_LOW_SLOW);  // (4)
	
  while (1)                                                // (5)
  {
    u16 i;                                                 // (6)
	
    for(i = 0; i < 60000; i++)                             // (7)
    {
      nop();                                               // (8)
    }

    GPIO_WriteReverse(GPIOB, GPIO_PIN_5);                  // (9)
  }                                                        // (10)
}                                                          // (11)

