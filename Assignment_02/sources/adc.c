/*
 * File:   adc.c
 * Author: Najat Mansour
 *
 * Created on November 26, 2023, 12:32 AM
 */


#include <xc.h>

void init_adc(void) {
    ADCON0 = 0;
    ADCON0bits.ADON = 1; // turn ADC on 
    ADCON2 = 0b10001001; // ADFM= 1 right justified 10 bits, 2 Tad, Fosc/8
}

int read_adc_raw_value(unsigned char channel) {
    int raw_value; 
    ADCON0bits.CHS = channel & 0x0F;
   
    //start conversion
    ADCON0bits.GO = 1;          
    while (ADCON0bits.GO) {}; 
    raw_value = ADRESH << 8 | ADRESL;  
    
    return raw_value; // 16 bit , it is actually 10 bit 0 ---1023
}

float read_adc_voltage(unsigned char channel) {
    int raw_value;
    float voltage;
    raw_value = read_adc_raw_value(channel);
    voltage = (raw_value * 5) / 1023.0;
    return voltage;
}