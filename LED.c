/*
 * LED.c
 * Tejaswini Kalikiri, Azra Gunes
 * Modified: 12/02/2024
 */
#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
// LaunchPad.h defines all the indices into the PINCM table

// initialize your LEDs
void LED_Init(void){
        // write this
        IOMUX->SECCFG.PINCM[PA16INDEX] = 0x00000081; // regular output
        GPIOA->DOE31_0 |= 0x00010000; // enable

        IOMUX->SECCFG.PINCM[PA17INDEX] = 0x00000081; // regular output
        GPIOA->DOE31_0 |= 0x00020000; // enable

        IOMUX->SECCFG.PINCM[PA24INDEX] = 0x00000081; // regular output
        GPIOA->DOE31_0 |= 0x01000000; // enable

///////////////////////////////////////////////////////////////////////////////////////

        IOMUX->SECCFG.PINCM[PB12INDEX] = 0x00000081; // regular output
        GPIOB->DOE31_0 |= 0x00001000; // enable

        IOMUX->SECCFG.PINCM[PB13INDEX] = 0x00000081; // regular output
        GPIOB->DOE31_0 |= 0x00002000; // enable

        IOMUX->SECCFG.PINCM[PB23INDEX] = 0x00000081; // regular output
        GPIOB->DOE31_0 |= 0x00800000; // enable

}
// data specifies which LED to turn on
void LED_On(uint32_t data){
    // write this
    uint32_t red = ((data & 0x000000F0)>>4);
    uint32_t blue = (data & 0x0000000F);
    if(red == 0x01)
    {
        GPIOA->DOUTSET31_0 = 1<<16;
    }
    else if (red == 0x02)
    {
        GPIOA->DOUTSET31_0 = 1<<17;
    }
    else if(red == 0x03)
    {
        GPIOA->DOUTSET31_0 = 1<<24;
    }

    if(blue == 0x01)
    {
        GPIOB->DOUTSET31_0 = 1<<12;
    }
    else if(blue == 0x02)
    {
        GPIOB->DOUTSET31_0 = 1<<13;
    }
    else if(blue == 0x03)
    {
        GPIOB->DOUTSET31_0 = 1<<23;
    }
    // use DOUTSET31_0 register so it does not interfere with other GPIO
}

// data specifies which LED to turn off
void LED_Off(uint32_t data){
    // write this
    // use DOUTCLR31_0 register so it does not interfere with other GPIO
    GPIOA->DOUTCLR31_0 = (1<<16)|(1<<17)|(1<<24);
    GPIOB->DOUTCLR31_0 = (1<<12)|(1<<13)|(1<<23);
}

// data specifies which LED to toggle
void LED_Toggle(uint32_t data){
    // write this
    // use DOUTTGL31_0 register so it does not interfere with other GPIO

}
