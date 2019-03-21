/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "clock_controller.h"
#include "timer.h"

/** @addtogroup Template_Project
  * @{
  */
void GPIO_setup(void);
void Switch_setup(void);//PB5,10
void GoStanbyMode(void);
void RCC_Config(void);


/* Private user define functions ---------------------------------------------------------*/
void delay(unsigned long ms);
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */

RTC_InitTypeDef   RTC_InitStructure;
RTC_TimeTypeDef   RTC_TimeStructure;

RTC_AlarmTypeDef  RTC_AlarmStructure;

void GetandSetWakeupAlarm(void){

    RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);

    /* Set the alarm X+5s */
    RTC_AlarmStructure.RTC_AlarmTime.RTC_H12     = RTC_TimeStructure.RTC_H12;
    RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours   = RTC_TimeStructure.RTC_Hours;
    if(RTC_TimeStructure.RTC_Seconds + 0x5 >=59){

      RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = RTC_TimeStructure.RTC_Minutes+1;
    RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = 0;

    }else{
      RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = RTC_TimeStructure.RTC_Minutes;
    RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = (RTC_TimeStructure.RTC_Seconds + 0x5) % 60;

    }
    RTC_AlarmStructure.RTC_AlarmDateWeekDay = 0x31;
    RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date;
    RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;


}




void SetAlarmAndSleep(void){
   /* Disable the Alarm A */
    RTC_AlarmCmd(RTC_Alarm_A, DISABLE);
    

    //* Already set by function above */
    RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStructure);

    /* Clear PWR WakeUp flag */
    PWR_ClearFlag(PWR_FLAG_WU);

    /* Clear RTC Alarm A flag */ 
    RTC_ClearFlag(RTC_FLAG_ALRAF);
  
    /* Enable RTC Alarm A Interrupt: this Interrupt will wake-up the system from
       STANDBY mode (RTC Alarm IT not enabled in NVIC) */
    RTC_ITConfig(RTC_IT_ALRA, ENABLE);
  
    /* Enable the Alarm A */
    RTC_AlarmCmd(RTC_Alarm_A, ENABLE);
    
    /* Request to enter STANDBY mode */
    PWR_EnterSTANDBYMode();

}


void GoStanbyMode(void){


  Switch_setup();

  SystemCoreClockUpdate();

  RCC_LSEConfig(RCC_LSE_OFF);
  RCC_LSICmd(DISABLE);
  RCC_HSICmd(DISABLE);
  RCC_HSEConfig(RCC_HSE_OFF);

  PWR_EnterSTANDBYMode();
    

}
int main(void)
{
  RCC_setup_HSI();
  // RCC_setup_MSI(RCC_MSIRange_0);
  RCC_Config();
  /* Enable WKUP pin 1 */
  // PWR_WakeUpPinCmd(PWR_WakeUpPin_1, ENABLE);  

  delay(500);
  GetandSetWakeupAlarm();
  SetAlarmAndSleep();
  while(1){
  }
}


void RCC_Config(void)
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

  /* Allow access to RTC Domain */
  PWR_RTCAccessCmd(ENABLE);

  /* Check if the StandBy flag is set */
  if (PWR_GetFlagStatus(PWR_FLAG_SB) != RESET)
  {
    /* Clear StandBy flag */
    PWR_ClearFlag(PWR_FLAG_SB);

    /* Turn on LED2 */
    // STM_EVAL_LEDOn(LED2); ////DO SOMETHING
    
    /* Wait for RTC APB registers synchronisation */
    RTC_WaitForSynchro();
    /* No need to configure the RTC as the RTC config(clock source, enable,
       prescaler,...) are kept after wake-up from STANDBY */
  }else if(PWR_GetFlagStatus(PWR_FLAG_WU) != RESET){

    PWR_ClearFlag(PWR_FLAG_WU);

    RTC_WaitForSynchro();

  }
  else
  {
    /* RTC Configuration ******************************************************/
    /* Reset RTC Domain */
    RCC_RTCResetCmd(ENABLE);
    RCC_RTCResetCmd(DISABLE);

    /* Enable the LSE OSC */
    RCC_LSEConfig(RCC_LSE_ON);
    
    /* Wait till LSE is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
    {}

    /* Select the RTC Clock Source */
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

    /* Enable the RTC Clock */
    RCC_RTCCLKCmd(ENABLE);

    /* Wait for RTC APB registers synchronisation */
    RTC_WaitForSynchro();

    /* Set the RTC time base to 1s */
    RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
    RTC_InitStructure.RTC_AsynchPrediv = 0x7F;
    RTC_InitStructure.RTC_SynchPrediv = 0x00FF;
    RTC_Init(&RTC_InitStructure);


    /* Set the time to 01h 00mn 00s AM */
    RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
    RTC_TimeStructure.RTC_Hours   = 0x01;
    RTC_TimeStructure.RTC_Minutes = 0x00;
    RTC_TimeStructure.RTC_Seconds = 0x00;  
    RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);
  }

  /* Clear RTC Alarm A flag */ 
  RTC_ClearFlag(RTC_FLAG_ALRAF);
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

