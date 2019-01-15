/**
 * an include wrapper file to switch between the STM8 families
 *
 * This helper file includes the right stm8xxx.h file depending on the used
 * CPU type.
 */

#if defined(STM8S208) || defined(STM8S207) || defined(STM8S105) || \
    defined(STM8S103) || defined(STM8S903) || defined(STM8AF52Ax) || \
    defined(STM8AF62Ax) || defined(STM8AF626x) || defined(STM8S007) || \
    defined(STM8S003)|| defined(STM8S005) || defined(STM8S001) || defined(STM8AF622x) 

 // check for STM8S family
 #include "stm8s.h"

#elif defined (STM8L15X_MD) || defined (STM8L15X_MDP) || defined (STM8L15X_HD) || defined (STM8L15X_LD) \
|| defined (STM8L05X_LD_VL) || defined (STM8L05X_MD_VL) || defined (STM8L05X_HD_VL) || defined (STM8AL31_L_MD)

 // compile for STM8L15x family
 #include "stm8l15x.h"

#else
 #error "Please select first the target device used in your application"
#endif
