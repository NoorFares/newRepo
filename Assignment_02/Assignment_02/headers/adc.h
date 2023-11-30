#ifndef ADC
#define	ADC

#include <xc.h> 

void init_adc(void);
int read_adc_raw_value(unsigned char channel);
float read_adc_voltage(unsigned char channel);

#endif	