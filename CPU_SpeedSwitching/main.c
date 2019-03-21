/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "clock_controller.h"
#include "timer.h"

/** @addtogroup Template_Project
  * @{
  */
void GPIO_setup(void);
void Switch_setup(void);//PB5,10

/* Private user define functions ---------------------------------------------------------*/
void delay(unsigned long ms);
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  // RCC_setup_HSI();
  // RCC_setup_MSI(RCC_MSIRange_0);

    // RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
  Switch_setup();
  // RCC_SYSCLKConfig(RCC_SYSCLKSource_MSI);
  SystemCoreClockUpdate();
  // GPIO_setup();
  RCC_LSEConfig(RCC_LSE_OFF);
  RCC_LSICmd(DISABLE);
    RCC_HSICmd(DISABLE);
    RCC_HSEConfig(RCC_HSE_OFF);
    // while(RCC_GetFlagStatus(RCC_FLAG_HSERDY) == SET);
    // delay(10000);
    PWR_EnterSTANDBYMode();
    
    // PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
  while(1);
  // TIMER4_Init();
  while (1)
  {
    /* LED at PB9 ON */
  RCC_setup_MSI(RCC_MSIRange_0);
  SystemCoreClockUpdate();
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
    delay(1000);
    GPIO_SetBits(GPIOA,GPIO_Pin_3);
    /* Delay 0.5 sec */
    // TIM4->ARR++;


    // if(TIM4->ARR > 100){
    //   TIM4 -> ARR = 0;
    // }

    // delay(1000);

  RCC_setup_MSI(RCC_MSIRange_0);
  SystemCoreClockUpdate();
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,DISABLE);
    /* LED at PB9 OFF */
    delay(20);
    GPIO_ResetBits(GPIOA,GPIO_Pin_3);
    //  Delay 0.5 sec 
    // delay(500);
  }
}

void GPIO_setup(void)
{
  /* GPIO Sturcture */
  GPIO_InitTypeDef GPIO_InitStructure;
  /* Enable Peripheral Clock AHB for GPIOB */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
  /* Configure PC13 as Output push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 |GPIO_Pin_2|GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

  GPIO_Init(GPIOA, &GPIO_InitStructure);
}
void Switch_setup(void)
{
  /* GPIO Sturcture */
  GPIO_InitTypeDef GPIO_InitStructure;
  /* Enable Peripheral Clock AHB for GPIOB */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE);
  /* Configure PC13 as Output push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 |GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;

  GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOB,GPIO_Pin_5 |GPIO_Pin_10);

}
// delay 1 ms per count @ Crystal 16.0 MHz 
void delay(unsigned long ms)
{
  volatile unsigned long i,j;
  for (i = 0; i < ms; i++ )
  for (j = 0; j < 1227; j++ );
}

