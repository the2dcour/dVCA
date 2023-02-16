//#include <stdio.h>
#include <ch32v00x_conf.h>
#include "debug.h"

/*********************************************************************
 * @fn      ADC_Function_Init
 *
 * @brief   Initializes ADC collection.
 *
 * @return  none
 */
void ADC_Function_Init(void)
{
    ADC_InitTypeDef  ADC_InitStructure = {0};
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    ADC_DeInit(ADC1);
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStructure);

    ADC_Calibration_Vol(ADC1, ADC_CALVOL_50PERCENT);
    ADC_DMACmd(ADC1, ENABLE);
    ADC_Cmd(ADC1, ENABLE);

    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));
}

/*********************************************************************
 * @fn      Get_ADC_Val
 *
 * @brief   Returns ADCx conversion result data.
 *
 * @param   ch - ADC channel.
 *            ADC_Channel_0 - ADC Channel0 selected.
 *            ADC_Channel_1 - ADC Channel1 selected.
 *            ADC_Channel_2 - ADC Channel2 selected.
 *            ADC_Channel_3 - ADC Channel3 selected.
 *            ADC_Channel_4 - ADC Channel4 selected.
 *            ADC_Channel_5 - ADC Channel5 selected.
 *            ADC_Channel_6 - ADC Channel6 selected.
 *            ADC_Channel_7 - ADC Channel7 selected.
 *            ADC_Channel_8 - ADC Channel8 selected.
 *            ADC_Channel_9 - ADC Channel9 selected.
 *
 * @return  none
 */
u16 Get_ADC_Val(u8 ch)
{
    u16 val;

    ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_241Cycles);
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    printf("Getting ADC Value\n");
    int grr = ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC);
    printf("\nGrr:%d", grr);

    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
    val = ADC_GetConversionValue(ADC1);
    printf("ADC Value: %d\n", val);
    return val;
}


int main(void) {
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
         Delay_Init();
         USART_Printf_Init(115200);
         printf("Initializing");
    while(1)
        {
            Delay_Ms(100);
  // Initialize the ADC
  //  ADC_Function_Init();

  // Read the ADC value from the specified channel
  int adcValue = Get_ADC_Val(ADC_Channel_0);

  // Convert 10-bit value to 9-bit value
  int convertedValue = (adcValue >> 1) & 0x1FF;

  // Assign each of the 9 bits to a set of 9 variables
  int bit0 = (convertedValue >> 0) & 1;
  int bit1 = (convertedValue >> 1) & 1;
  int bit2 = (convertedValue >> 2) & 1;
  int bit3 = (convertedValue >> 3) & 1;
  int bit4 = (convertedValue >> 4) & 1;
  int bit5 = (convertedValue >> 5) & 1;
  int bit6 = (convertedValue >> 6) & 1;
  int bit7 = (convertedValue >> 7) & 1;
  int bit8 = (convertedValue >> 8) & 1;

  // Print the 9 binary values

  printf("%d%d%d%d%d%d%d%d%d\n", bit8, bit7, bit6, bit5, bit4, bit3, bit2, bit1, bit0);
        }

  return 0;
}

