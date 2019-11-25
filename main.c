/*
 * File:   main.c
 * Author: Raff
 *
 * Created on 9 september 2019, 17:22
 */


//includes
#include <p33CK256MP508.h>
#include "xc.h"
#include "system.h"
#include "adc.h"
#include "pwm.h"
#include "comp.h"




//variables


int main(void) {
    TRISB = 0;
    TRISEbits.TRISE6 = 0;
    TRISEbits.TRISE7=1;
    TRISEbits.TRISE8=1;
    TRISEbits.TRISE9=1;
    TRISEbits.TRISE5=0;
    
  
    ClockShift ();

    D0ADCConfig();
    //D1ADCConfig();
    //ShADCConfig();
    FullBridgePWM();
    HalfBridgePWM();
    //CompConfig();
    
    //start ADC convertion
    //ADCON3Lbits.CNVRTCH = 1; //to start ADC conversion
    
    while(1){
        
        
        Update();
        //buttons polling
        if(PORTEbits.RE7==0){ //button RE7
           
        }else{
           
        }
        if(PORTEbits.RE8==0){ //button RE8
          
        }else{
           
        }
        if(PORTEbits.RE9==0){ //button RE9
           
        }else{
           
        }
    }
    return 0;
}


