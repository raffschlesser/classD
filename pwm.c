/*
 * File:   ADC.c
 * Author: Raff
 *
 * Created on 22 september 2019, 13:37
 */


#include "xc.h"

void HalfBridgePWM(void){
 
    //PWM control register configuration
    PCLKCONbits.MCLKSEL = 1; // AFPLLO ? Auxiliary PLL post-divider output
    PCLKCONbits.DIVSEL = 0 ;
    PG1CONLbits.CLKSEL = 1; //PWM Generator uses Master clock selected by the MCLKSEL[1:0] (PCLKCON[1:0]) control bits
    PG1CONLbits.HREN = 1; // PWM Generator x operates in High-Resolution mode (resolution of 250ps)
    PG1CONLbits.MODSEL = 0b000; //Independent edge triggered mode 
    PG1CONH = 0x0000;
    //PWM uses PG1DC, PG1PER, PG1PHASE registers
    //PWM Generator does not broadcast UPDATE status bit state or EOC signal
    //Update the data registers at start of next PWM cycle (SOC) 
    //PWM Generator operates in Single Trigger mode
    //Start of cycle (SOC) = local EOC
    PG1IOCONH = 0x000C;
    //PWM Generator outputs operate in Complementary mode
    //PWM Generator controls the PWM1H and PWM1L output pins
    //PWM1H & PWM1L output pins are active high
    PG1EVTLbits.UPDTRG = 1; // A write of the PGxDC register automatically sets the UPDATE bit
    PG1EVTLbits.ADTR1PS = 0; //0; //1:1 postscaler Trigger 1
    PG1EVTLbits.ADTR1EN1 = 1; //PGxTRIGA for triggering trigger 1
    //Write to DATA REGISTERS
    PG1PER = 0x435a;//0x21a0;// //PWM frequency is 100kHz
    PG1DC = 0x0000; // ?% duty
    PG1PHASE = 0x0010; //Phase offset in rising edge of PWM
    PG1DTH = 145; //Dead time on PWMH   250ps * 400 = 100 ns
    PG1DTL = 145; //Dead time on PWML   250ps * 400 = 100 ns
    PG1TRIGA = 0x0010;
    //Enable PWM
    PG1CONLbits.ON = 1; //PWM module is enabled

}
void FullBridgePWM(void){
 
    //PWM control register configuration
    PCLKCONbits.MCLKSEL = 1; //  AFVCO/2 ? Auxiliary VCO/2 500MHz
    PCLKCONbits.DIVSEL = 0 ; // 1:1
    
    PG3CONLbits.CLKSEL = 1; // PWM Generator uses Master clock selected by the MCLKSEL[1:0] (PCLKCON[1:0]) control bits)
    PG3CONLbits.HREN = 1; // PWM Generator x operates in High-Resolution mode
    PG3CONLbits.MODSEL = 0b000; //Independent edge triggered mode 
    PG3CONH = 0x0001;
    //PWM uses PG3DC, PG3PER, PG3PHASE registers
    //PWM Generator does not broadcast UPDATE status bit state or EOC signal
    //Update the data registers at start of next PWM cycle (SOC) 
    //Trigger output selected by PG1 Start of cycle (SOC) = local EOC
    PG3IOCONH = 0x000c;
    //PWM Generator outputs operate Independent in mode
    //PWM Generator controls the PWM3H and not PWM3L output pins
    //PWM3H & PWM3L output pins are active high
    //Write to DATA REGISTERS
    PG3PER = 0x435a; //0x21a0;////PWM frequency
    PG3DC = 0x0010; // 0% duty
    PG3PHASE = 0x0010; //Phase offset in rising edge of PWM
    PG3DTH = 145; //Dead time on PWMH   250ps * 400 = 100 ns
    PG3DTL = 145; //Dead time on PWML   250ps * 400 = 100 ns
    PG3EVTLbits.UPDTRG = 1; // A write of the PGxDC register automatically sets the UPDATE bit
    
    PG4CONLbits.CLKSEL = 1; // PWM Generator uses Master clock selected by the MCLKSEL[1:0] (PCLKCON[1:0]) control bits)
    PG4CONLbits.HREN = 1; // PWM Generator x operates in High-Resolution mode
    PG4CONLbits.MODSEL = 0b000; //Independent edge triggered mode 
    PG4CONH = 0x0001;
    //PWM uses PG2DC, PG2PER, PG2PHASE registers
    //PWM Generator does not broadcast UPDATE status bit state or EOC signal
    //Update the data registers at start of next PWM cycle (SOC) 
    //Trigger output selected by PG1 Start of cycle (SOC) = local EOC
    PG4IOCONH = 0x000c;
    //PWM Generator outputs operate Independent in mode
    //PWM Generator controls the PWM2H and not PWM2L output pins
    //PWM2H & PWM2L output pins are active high
    PG4EVTLbits.UPDTRG = 1; // A write of the PGxDC register automatically sets the UPDATE bit
   // PG2EVTLbits.ADTR1PS = 2; //1:3 postscaler Trigger 1
   // PG2EVTLbits.ADTR1EN1 = 1; //PGxTRIGA for triggering trigger 1
    //Write to DATA REGISTERS
    //PG2PER = 0x1fff; //PWM frequency
    PG4PER = 0x435a;//0x21a0;// //PWM frequency
    PG4DC = 0x0000; // 0% duty
    PG4PHASE = 0x0010; //Phase offset in rising edge of PWM
    PG4DTH = 145; //Dead time on PWMH   250ps * 400 = 100 ns
    PG4DTL = 145; //Dead time on PWML   250ps * 400 = 100 ns
    
    //Enable PWM
    PG3CONLbits.ON = 1; //PWM module is enabled
    PG4CONLbits.ON = 1; //PWM module is enabled
    
}