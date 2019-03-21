
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CLOCK_CONTROLLER_H
#define __CLOCK_CONTROLLER_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx.h"
#include "stm32l1xx_conf.h"


/* Private setup functions ---------------------------------------------------------*/
void RCC_setup_HSI(void);
void RCC_setup_MSI(void);


#endif 

