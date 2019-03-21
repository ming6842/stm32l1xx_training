/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "clock_controller.h"
#include "timer.h"

/** @addtogroup Template_Project
  * @{
  */
void GPIO_setup(void);

/* Private user define functions ---------------------------------------------------------*/
void delay(unsigned long ms);
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  RCC_setup_HSI();
  GPIO_setup();
  TIMER4_Init();
  while (1)
  {
    /* LED at PB9 ON */
    GPIO_SetBits(GPIOA,GPIO_Pin_2 | GPIO_Pin_3);
    /* Delay 0.5 sec */
    delay(5);
    /* LED at PB9 OFF */
    GPIO_ResetBits(GPIOA,GPIO_Pin_2 | GPIO_Pin_3);
    //  Delay 0.5 sec 
    delay(5);
  }
}

void GPIO_setup(void)
{
  /* GPIO Sturcture */
  GPIO_InitTypeDef GPIO_InitStructure;
  /* Enable Peripheral Clock AHB for GPIOB */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
  /* Configure PC13 as Output push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

  GPIO_Init(GPIOA, &GPIO_InitStructure);
}
// delay 1 ms per count @ Crystal 16.0 MHz 
void delay(unsigned long ms)
{
  volatile unsigned long i,j;
  for (i = 0; i < ms; i++ )
  for (j = 0; j < 1227; j++ );
}

