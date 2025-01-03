/*
 * Switch.c
 * Tejaswini Kalikiri, Azra Gunes
 * Modified: 12/02/2024
 */
#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
// LaunchPad.h defines all the indices into the PINCM table
void Switch_Init(void){
    IOMUX->SECCFG.PINCM[PB19INDEX] = 0x00040081; // switch 3
    IOMUX->SECCFG.PINCM[PA25INDEX] = 0x00040081; // switch 4
    IOMUX->SECCFG.PINCM[PB17INDEX] = 0x00040081; // switch 2
    IOMUX->SECCFG.PINCM[PB16INDEX] = 0x00040081; // switch 1
  
}
// return current state of switches
uint32_t Switch_InA(void){
    uint32_t input;
    input = (GPIOA->DIN31_0 & (0x02000000))>>25;
    return input;
}

uint32_t Switch_InB(void){
    uint32_t input;
        input = (GPIOB->DIN31_0 & (0x0F<<16))>>16;
        return input;
}
