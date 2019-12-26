/*
 * File:   pwm.c
 * Author: Raff
 *
 * pwm.c is used to config the high resulution PWM modules
 * period, dutycylce phase and deadtime can be configered at the top 
 * and is at the start of theprogram set the same for every PWM module
 * later duty cycle will be changed in the ADC interupts
 */


#include "xc.h"

#define PWMperiod 0x451D;
#define PWMdeadtime 145;
#define PWMphase 0x10;
#define PWMduty 0x0010;



void HalfBridgePWM(void){
    //original writen to drive half bridge topology 
    //but now used for sicing fron the full bridge pwm mudules
 
    //PWM control register configuration
    PCLKCONbits.MCLKSEL = 1; // AFVCO/2 Auxiliary PLL post-divider output
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
    PG1EVTLbits.ADTR1PS = 1; //1:1 postscaler Trigger 1
    PG1EVTLbits.ADTR1EN1 = 1; //PGxTRIGA for triggering trigger 1
    //Write to DATA REGISTERS
    PG1PER = PWMperiod;//0x21a0;// //PWM frequency is 100kHz
    PG1DC = PWMduty; // ?% duty
    PG1PHASE = PWMphase; //Phase offset in rising edge of PWM
    PG1DTH = PWMdeadtime; //Dead time on PWMH   250ps * 400 = 100 ns
    PG1DTL = PWMdeadtime; //Dead time on PWML   250ps * 400 = 100 ns
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
    PG3PER = PWMperiod;////PWM frequency
    PG3DC = PWMduty; // 0% duty
    PG3PHASE = PWMphase; //Phase offset in rising edge of PWM
    PG3DTH = PWMdeadtime; //Dead time on PWMH   250ps * 400 = 100 ns
    PG3DTL = PWMdeadtime; //Dead time on PWML   250ps * 400 = 100 ns
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
    PG4PER = PWMperiod;//PWM frequency
    PG4DC = PWMduty; // DC/PER = ...% duty
    PG4PHASE = PWMphase; //Phase offset in rising edge of PWM
    PG4DTH = PWMdeadtime; //Dead time on PWMH   250ps * DT = ... ns
    PG4DTL = PWMdeadtime; //Dead time on PWML   250ps * DT = ... ns
    
    //Enable PWM
    PG3CONLbits.ON = 1; //PWM module is enabled
    PG4CONLbits.ON = 1; //PWM module is enabled
    
}