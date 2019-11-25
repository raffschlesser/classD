/*
 * File:   comp.c
 * Author: Raff
 *
 * Created on 12 october 2019, 13:37
 */


#include <p33CK256MP508.h>

#include "xc.h"

void CompConfig(){
    //PWM
    PG8DC = 0; /*PWM duty cycle set to 0*/
    /*PCI logic configuration for Hysteretic mode, comparator 1 output as PCI source*/
    PG8CLPCIL = 0x123B; /* ACMP1 out selected as PCI input, latched PCI acceptance */
    PG8CLPCIH = 0x0300; /* Latched PCI signal as acceptance criteria */
    PG8LEBL = 0x0100; /* LEB Count = 0x100 */
    /*Enable PWM macro*/
    PG8CONLbits.ON = 1; /*PWM module is enabled*/
    /* Triangle Wave Mode Settings 
     DAC Register Settings */
    DACCTRL1Lbits.CLKSEL = 3;
    DAC1DATHbits.DACDATH = 0xffe; /* DAC Output set to 1V (AVDD = 3.3V)*/
    DAC1DATLbits.DACDATL = 0; //Lower data for TWM
    
    SLP1DATbits.SLPDAT = 100; //Slope data rate 
    SLP1CONHbits.SLOPEN = 1; //Enable Slope 
    SLP1CONHbits.TWME = 1; //Enable Triangle Mode 
  
    DAC1CONLbits.INSEL = 1; /* Select CMP1B input */
    
    DAC1CONLbits.DACOEN = 1; /* Enable DAC 1 output on pin DACOUT1 */
    DAC1CONLbits.DACEN = 1; /* Enable Master DAC 1 */
    DACCTRL1Lbits.DACON = 1; /* Turn ON all Master DACs */
    
}