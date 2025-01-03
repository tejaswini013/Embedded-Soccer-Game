// FIFO1.c
// Runs on any microcontroller
// Provide functions that implement the Software FiFo Buffer
// Last Modified: 04/08/2024
// Student names: Tejaswini Kalikiri, Azra Gunes
// Last modification date: 12/02/2024
#include <stdint.h>


// Declare state variables for FiFo
// size, buffer, put and get indexes
#define SIZE 64
int32_t static put_i;
int32_t static get_i;
char static FIFO[SIZE];

// *********** Fifo1_Init**********
// Initializes a software FIFO1 of a
// fixed size and sets up indexes for
// put and get operations

void Fifo1_Init(){
//Complete this
  put_i = get_i = 0;
}

// *********** Fifo1_Put**********
// Adds an element to the FIFO1
// Input: data is character to be inserted
// Output: 1 for success, data properly saved
//         0 for failure, FIFO1 is full
uint32_t Fifo1_Put(char data){
    int32_t next;
    if(put_i == SIZE-1)
        next = 0;
    else
        next = put_i + 1;
    if(next == get_i)
        return 0;
    FIFO[put_i] = data;
    put_i = next;
    return 1;
}

// *********** Fifo1_Get**********
// Gets an element from the FIFO1
// Input: none
// Output: If the FIFO1 is empty return 0
//         If the FIFO1 has data, remove it, and return it
char Fifo1_Get(void){
    char value;
    if(put_i == get_i)
        return 0;
    value = FIFO[get_i];
    if(get_i == SIZE-1)
        get_i = 0;
    else get_i += 1;
    return value;
}



