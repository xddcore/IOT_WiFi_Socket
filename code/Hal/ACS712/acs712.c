#include "acs712.h"
#include "adc.h"
unsigned long Electric;
int get_Electric()
{
u16 adcx;
float temp;
	adcx=Get_Adc_Average(ADC_Channel_1,10);
	temp=(float)adcx*(3.3/4096);
	adcx=temp;
	temp-=adcx;
if((adcx+temp)>(base_vlotage/1000))	{Electric=(int)((((adcx+temp)*1000)-base_vlotage)/acs712_30a); return Electric;}//得到电流值 单位A
if((adcx+temp)<(base_vlotage/1000))	{Electric=(int)((base_vlotage-((adcx+temp)*1000))/acs712_30a);return Electric;}//得到电流值 单位A	
if((adcx+temp)==(base_vlotage/1000) )  { return 0;}
//return (adcx+temp);
}

