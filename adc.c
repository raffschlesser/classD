/*
 * File:   ADC.c
 * Author: Raff
 *
 * Created on 22 september 2019, 13:37
 */

//#define FEEDBACK
#define PWMMODE

#include "xc.h"
static int16_t running[2]={2048,2048};
static int16_t dataADC0=0;
static int16_t dataADC1=0;
static int16_t data=0xfff;
static int16_t sampled=0;
int16_t gain=1;
double integral=0;
int16_t error=0;

void D0ADCConfig(void){
    // dedicated core 0 ADC INITIALIZATION
    // Configure the I/O pins to be used as analog inputs.
    ANSELAbits.ANSELA0 = 1; 
    TRISAbits.TRISA0 = 1; // AN23/RE3 connected the shared core for potentiometer devboard
    // Configure the common ADC clock.
     ADCON3Hbits.CLKSEL = 3; // clock from FVCO/4    250MHz
    ADCON3Hbits.CLKDIV = 3; // no clock divider (1:4)   62.5MHz 
    // Configure the cores? ADC clock.
    ADCORE0Hbits.ADCS = 0; // dedicated core clock divider (1:2) (max is 70MHz)
    // Configure sample time for shared core.
    ADCON4L = 3; //Dedicated ADC Core 0 and 1 Conversion Delay Enabled
    ADCORE0Lbits.SAMC = 0; // 12 TAD sample time
    // Configure the ADC reference sources.
    ADCON3Lbits.REFSEL = 0; // AVdd and AVss as voltage reference
    //sellect resolution
    ADCORE0Hbits.RES = 3; // 12 bit resolution
    // Configure the integer of fractional output format.
    ADCON1Hbits.FORM = 0; // integer format
    // Select single-ended input configuration and unsigned output format.
    //defuald is Channel is single-ended and Channel output data are unsigned
    ADMOD0Lbits.SIGN0 = 0; // AN0/RA0
    ADMOD0Lbits.DIFF0 = 0; // AN0/RA0


    // Configure and enable early ADC interrupts.
    ADCORE0Hbits.EISEL = 0; // early interrupt is generated 1 TADCORE clock prior to when the data is ready
    ADCON2Lbits.EIEN = 1; // enable early interrupts for ALL inputs
    ADEIELbits.EIEN0 = 1; // enable early interrupt for AN0 
    _ADCAN0IF = 0; // clear interrupt flag for AN23
    _ADCAN0IE = 1; // enable interrupt for AN23

    // Set initialization time to maximum
    ADCON5Hbits.WARMTIME = 15;
    // Turn on ADC module
    ADCON1Lbits.ADON = 1;
    // Turn on analog power for dedicated core 0
    ADCON5Lbits.C0PWR = 1;
    // Wait when the core 0 is ready for operation
    while(ADCON5Lbits.C0RDY == 0);
    // Turn on digital power to enable triggers to the core 0
    ADCON3Hbits.C0EN = 1;
    ADCON3L = (ADCON3L & 0xFE00) | 0; //select adc channel for conversion
    
    // Set same trigger source for all inputs to sample signals simultaneously.
    ADTRIG0Lbits.TRGSRC0 = 0b100; // PWM1 trig1 for AN0
    ADLVLTRGLbits.LVLEN0 = 0; // Input trigger is edge-sensitive
    //see PWM for PWM and trigger config
}

void D1ADCConfig(void){
    // dedicated core 0 ADC INITIALIZATION
    // Configure the I/O pins to be used as analog inputs.
    ANSELAbits.ANSELA0 = 1; 
    TRISAbits.TRISA0 = 1; // AN23/RE3 connected the shared core for potentiometer devboard
    // Configure the common ADC clock.
    ADCON3Hbits.CLKSEL = 3; // clock from FVCO/4    250MHz
    ADCON3Hbits.CLKDIV = 3; // no clock divider (1:4)   62.5MHz 
    // Configure the cores? ADC clock.
    ADCORE1Hbits.ADCS = 0; // dedicated core clock divider (1:2) (max is 70MHz)
    // Configure sample time for shared core.
    ADCON4L = 3; //Dedicated ADC Core 0 and 1 Conversion Delay Enabled
    ADCORE1Lbits.SAMC = 200; // 12 TAD sample time
    // Configure the ADC reference sources.
    ADCON3Lbits.REFSEL = 0; // AVdd and AVss as voltage reference
    //sellect resolution
    ADCORE1Hbits.RES = 3; // 12 bit resolution
    // Configure the integer of fractional output format.
    ADCON1Hbits.FORM = 0; // integer format
    // Select single-ended input configuration and unsigned output format.
    //defuald is Channel is single-ended and Channel output data are unsigned
    ADMOD0Lbits.SIGN1 = 0; // AN1/RB2
    ADMOD0Lbits.DIFF1 = 0; // AN1/RB2


    // Configure and enable early ADC interrupts.
    ADCORE1Hbits.EISEL = 0; // early interrupt is generated 1 TADCORE clock prior to when the data is ready
    ADCON2Lbits.EIEN = 1; // enable early interrupts for ALL inputs
    ADEIELbits.EIEN1 = 1; // enable early interrupt for AN0 
    _ADCAN1IF = 0; // clear interrupt flag for AN23
    _ADCAN1IE = 1; // enable interrupt for AN23

    // Set initialization time to maximum
    ADCON5Hbits.WARMTIME = 15;
    // Turn on ADC module
    ADCON1Lbits.ADON = 1;
    // Turn on analog power for dedicated core 0
    ADCON5Lbits.C1PWR = 1;
    // Wait when the core 0 is ready for operation
    while(ADCON5Lbits.C1RDY == 0);
    // Turn on digital power to enable triggers to the core 0
    ADCON3Hbits.C1EN = 1;
   // ADCON3L = (ADCON3L & 0xFE00) | 0; //select adc channel for conversion
    
    // Set same trigger source for all inputs to sample signals simultaneously.
    ADTRIG0Lbits.TRGSRC1 = 0b100; // PWM1 trig1 for AN1
    ADLVLTRGLbits.LVLEN1 = 0; // Input trigger is edge-sensitive
    //see PWM for PWM and trigger config
}

void ShADCConfig(void){
    // shared core ADC INITIALIZATION
    // Configure the I/O pins to be used as analog inputs.
    ANSELEbits.ANSELE3 = 1; 
    TRISEbits.TRISE3 = 1; // AN23/RE3 connected the shared core for potentiometer devboard
    // Configure the common ADC clock.
    ADCON3Hbits.CLKSEL = 3; // clock from FVCO/4    250MHz
    ADCON3Hbits.CLKDIV = 3; // no clock divider (1:4)   62.5MHz 
    // Configure the cores? ADC clock.
    ADCON2Lbits.SHRADCS = 0; // shared core clock divider (1:2) (max is 70MHz)
    // Configure sample time for shared core.
    ADCON2Hbits.SHRSAMC = 100;  // 12 TAD sample time
    // Configure the ADC reference sources.
    ADCON3Lbits.REFSEL = 0; // AVdd and AVss as voltage reference
    //sellect resolution
    ADCON1Hbits.SHRRES = 3; // 12 bit resolution
    // Configure the integer of fractional output format.
    ADCON1Hbits.FORM = 0; // integer format
    // Select single-ended input configuration and unsigned output format.
    //defuald is Channel is single-ended and Channel output data are unsigned
    ADMOD1Lbits.SIGN23 = 0; // AN23
    ADMOD1Lbits.DIFF23 = 0; // AN23


    // Configure and enable early ADC interrupts.
    ADCORE0Hbits.EISEL = 0; // early interrupt is generated 1 TADCORE clock prior
    // to when the data is ready
    ADCORE1Hbits.EISEL = 3; // early interrupt is generated 4 TADCORE clocks prior
    // to when the data is ready
    ADCON2Lbits.EIEN = 1; // enable early interrupts for ALL inputs
    ADEIEHbits.EIEN23 = 1; // enable early interrupt for AN23 (with potentiometer)
    _ADCAN23IF = 0; // clear interrupt flag for AN23
    _ADCAN23IE = 1; // enable interrupt for AN23

    // Set initialization time to maximum
    ADCON5Hbits.WARMTIME = 15;
    // Turn on ADC module
    ADCON1Lbits.ADON = 1;
    // Turn on analog power for shared core
    ADCON5Lbits.SHRPWR = 1;
    // Wait when the shared core is ready for operation
    while(ADCON5Lbits.SHRRDY == 0);
    // Turn on digital power to enable triggers to the shared core
    ADCON3Hbits.SHREN = 1;
    ADCON3L = (ADCON3L & 0xFE00) | 23; //select adc channel for conversion

    // Set same trigger source for all inputs to sample signals simultaneously.
  
    ADTRIG5Hbits.TRGSRC23 = 0b100; // PWM1 trig1 for AN23
    ADLVLTRGHbits.LVLEN23 = 0; // Input trigger is edge-sensitive
}

// ADC AN0 ISR (CORE 0)
void __attribute__((interrupt, no_auto_psv)) _ADCAN0Interrupt(void)
{
    dataADC0 = (ADCBUF0); // read conversion resul
   
    
    #ifndef FEEDBACK
 
    #ifndef PWMMODE
    PG1DC = dataADC0; //1;
    if(dataADC0&0x800){
        PG4DC = 0;
        PG3DC =(dataADC0&0x7ff); //2;
    }else{
        PG3DC = 0;
        PG4DC = (dataADC0^0x7ff); //2;
    }
    #else
    //dataADC0=(((dataADC0-2048)*2)+2048);
    PG1DC = dataADC0<<2;
    PG4DC = dataADC0<<2;
    PG3DC = ((dataADC0^0xfff)-0)<<2;
    if(LATEbits.LATE6==1){LATEbits.LATE6=0;}else{LATEbits.LATE6=1;}
    #endif
#else     
    sampled++;
#endif
    _ADCAN0IF = 0; // clear interrupt flag
}

// ADC AN1 ISR (CORE 1)
void __attribute__((interrupt, no_auto_psv)) _ADCAN1Interrupt(void)
{
    dataADC1 = (ADCBUF1); // read conversion resul
    #ifndef FEEDBACK
    #ifndef PWMMODE
    PG1DC = dataADC0; //1;
    if(dataADC0&0x800){
        PG4DC = 0;
        PG3DC =(dataADC0&0x7ff); //2;
    }else{
        PG3DC = 0;
        PG4DC = (dataADC0^0x7ff); //2;
    }
    #else
    PG1DC = dataADC1<<2;
    PG4DC = dataADC1<<2;
    PG3DC = (dataADC1^0xfff)<<2;
    if(LATEbits.LATE6==1){LATEbits.LATE6=0;}else{LATEbits.LATE6=1;}
    #endif
#else     
    sampled++;
#endif
    _ADCAN1IF = 0; // clear interrupt flag
}

// ADC AN23 ISR (SHARED CORE)
void __attribute__((interrupt, no_auto_psv)) _ADCAN23Interrupt(void)
{
    dataADC0 = ADCBUF23; // read conversion result
    //dataADC0 = 0x0fff;
    PG1DC = dataADC0;
    PG4DC = dataADC0;
    PG3DC = (dataADC0^0xfff)<<1;
    if(LATEbits.LATE6==1){LATEbits.LATE6=0;}else{LATEbits.LATE6=1;}
    _ADCAN23IF = 0; // clear interrupt flag
}

void Update(){
    #ifndef FEEDBACK
    /*#ifndef PWMMODE
    if (sampled ==1){
    PG1DC = dataADC0<<1; //1;
    if(dataADC0&0x800){
        PG4DC = 0;
        PG3DC =(dataADC0&0x7ff)<<2; //2;
    }else{
        PG3DC = 0;
        PG4DC = (dataADC0^0x7ff)<<2; //2;
    }
    LATEbits.LATE6=1;
    }else if (sampled>1) LATEbits.LATE5=1;
    #else
    PG1DC = dataADC0<<1; //1;
    PG4DC = dataADC0<<1;
    PG3DC = (dataADC0^0xfff)<<1;
    #endif*/
    #else
    if(sampled==2){
        sampled=0;
        
      
        dataADC1=(dataADC1^0xfff);
        error = error+ (dataADC1-dataADC0)>>2;
       // if(error!=0)LATEbits.LATE6=1;
        //integral = integral+(((double)error));
        if(integral>1000){
            integral=1000;
        }
        if(integral<-1000){
            integral=-1000;
        }
        
        if(LATEbits.LATE5==1){LATEbits.LATE5=0;}else{LATEbits.LATE5=1;}
        dataADC0 = dataADC0+error;//+(integral*0.001);*/
        
         
    #ifndef PWMMODE
    if (sampled ==1){
    PG1DC = dataADC0<<1; //1;
    if(dataADC0&0x800){
        PG4DC = 0;
        PG3DC =(dataADC0&0x7ff)<<2; //2;
    }else{
        PG3DC = 0;
        PG4DC = (dataADC0^0x7ff)<<2; //2;
    }
    LATEbits.LATE6=1;
    }else if (sampled>1) LATEbits.LATE5=1;
    #else
    PG1DC = dataADC0<<1; //1;
    PG4DC = dataADC0<<1;
    PG3DC = (dataADC0^0xfff)<<1;
    #endif
    }else if(sampled>2)LATEbits.LATE6=1; //error has happened
    #endif
}