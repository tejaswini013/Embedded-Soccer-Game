// Lab9Main.c
// Runs on MSPM0G3507
// Lab 9 ECE319K
// Tejaswini Kalikiri, Dongyoon Kim
// Last Modified: 12/02/2024

#include <stdio.h>
#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include "../inc/ST7735.h"
#include "../inc/Clock.h"
#include "../inc/LaunchPad.h"
#include "../inc/TExaS.h"
#include "../inc/Timer.h"
#include "../inc/ADC1.h"
#include "../inc/DAC5.h"
#include "SmallFont.h"
#include "LED.h"
#include "Switch.h"
#include "Sound.h"
#include "images/images.h"



#define ADCVREF_VDDA 0x000
#define FIX 16
// ****note to ECE319K students****
// the data sheet says the ADC does not work when clock is 80 MHz
// however, the ADC seems to work on my boards at 80 MHz
// I suggest you try 80MHz, but if it doesn't work, switch to 40MHz
void PLL_Init(void){ // set phase lock loop (PLL)
  // Clock_Init40MHz(); // run this line for 40MHz
  Clock_Init80MHz(0);   // run this line for 80MHz
}

uint32_t M=1; uint32_t N=1; uint32_t O=1; uint32_t P=1; uint32_t Q=1; uint32_t R=1;
/**     Random Functions       **/
uint32_t Random32_1(void){
  M = 1664525*M+1013904223;
  return M;
}
uint32_t Random32_2(void){
    N = 1839283*N + 1029298301;
    return N;
}
uint32_t Random32_3(void){
    O = 1839393*O + 10209236301;
    return O;
}
uint32_t Random32_4(void){
    P = 1809283*P + 9999228301;
    return P;
}
uint32_t Random32_5(void){
    Q = 1888883*Q + 1873568301;
    return Q;
}
uint32_t Random32_6(void){
    R = 1982271*R + 1732889912;
    return R;
}


uint32_t Random1(uint32_t n){
  return (Random32_1()>>16)%n;
}
uint32_t Random2(uint32_t n){
    return (Random32_2()>>16)%n;
}
uint32_t Random3(uint32_t n){
    return (Random32_3()>>16)%n;
}
uint32_t Random4(uint32_t n){
    return (Random32_4()>>16)%n;
}
uint32_t Random5(uint32_t n){
    return (Random32_5()>>16)%n;
}
uint32_t Random6(uint32_t n){
    return (Random32_6() >>16)%n;
}
/**   Declaration of players and ball   **/
struct player{
    uint32_t x;
    uint32_t y;
    uint32_t oldx;
    uint32_t oldy;
    const uint16_t *image;
    const uint16_t * bimage;
};
typedef struct player player_t;
player_t players[8] = {
                       {5*FIX,100*FIX,5*FIX,100*FIX,blue_player1,blank_player},
                       {55*FIX,100*FIX,55*FIX,100*FIX,blue_player2,blank_player},
                       {105*FIX,100*FIX,105*FIX,100*FIX,blue_player3,blank_player},
                       {55*FIX,131*FIX,55*FIX,131*FIX,blue_keeper,blank_player},
                       {5*FIX,60*FIX,5*FIX,60*FIX,red_player1,blank_player},
                       {55*FIX,60*FIX,55*FIX,60*FIX,red_player2,blank_player},
                       {105*FIX,60*FIX,105*FIX,60*FIX,red_player3,blank_player},
                       {55*FIX,42*FIX,55*FIX,42*FIX,red_keeper,blank_player}
};
struct ball{
    uint32_t ballx;
    uint32_t bally;
    uint32_t oldballx;
    uint32_t oldbally;
    const uint16_t *image;
    const uint16_t *bimage;
};
typedef struct ball ball_t;
ball_t soccerball = {55*FIX,80*FIX,55*FIX,80*FIX,soccerball_image,soccerball_blank};
/**    global variables     **/
uint32_t ReceiveCount, TransmitCount;
uint32_t Data;
uint32_t switchplayer_pressed;
uint32_t switchpass_pressed;
uint32_t switchshoot_pressed;
uint32_t switchreset_pressed;
uint32_t ADCX; //ADC0 channel1
uint32_t ADCY; //ADC0 channel0
uint32_t playernumber = 0;
uint32_t ball_vx, ball_vy;
uint32_t positive_x_flag, positive_y_flag;
uint32_t dribble, passandshoot_cnt;
uint8_t chosenLang;
uint32_t shoot_cnt;
uint32_t pass_cnt;
uint32_t red_dribble;
uint32_t red_passandshoot_cnt;
uint32_t attack = 1;
uint32_t defend = 1;
uint32_t move_right = 1;
uint32_t move_left = 1;
/*****            Move Function               *********/
void Move(uint32_t ADCX,uint32_t ADCY)
{
        /**     player move     **/
        uint32_t player_posx, player_posy;
        uint32_t random_arr[6];
        random_arr[0] = Random1(4);
        random_arr[1] = Random2(4);
        random_arr[2] = Random3(8);
        random_arr[3] = Random4(8);
        random_arr[4] = Random5(8);
        random_arr[5] = Random6(4);
        uint32_t arr_index = 0;
        if(ADCX > 2047)     //-x
        {
            player_posx = ConvertNegative(ADCX);
            if(((players[playernumber].x+10*FIX)-player_posx) >= 10*FIX)
                players[playernumber].x -= player_posx;
        }
        else                //+x
        {
            player_posx = ConvertPositive(ADCX);
            if((players[playernumber].x+10*FIX+player_posx) <= 127*FIX)
                players[playernumber].x += player_posx;
        }
        if(ADCY > 2047)     //-y
        {
            player_posy = ConvertNegative(ADCY);
            if(players[playernumber].y-14*FIX-player_posy >=(28+8)*FIX)
                players[playernumber].y -= player_posy;
        }
        else                //+y
        {
            player_posy = ConvertPositive(ADCY);
            if((players[playernumber].y + player_posy) <=(130-8)*FIX)
                players[playernumber].y += player_posy;
        }
        for(int i=0; i<7;i++)
        {
            //random movement
            if((i != playernumber)&&(i < 3)) //blue_players
            {
                if(random_arr[arr_index] == 0) //move left
                {
                    if(((players[i].x+10*FIX)-FIX) >= 10*FIX)
                        players[i].x -= FIX;
                }
                if(random_arr[arr_index] == 1) //move right
                {
                    if((players[i].x+10*FIX + FIX) <= 127*FIX)
                        players[i].x += FIX;
                }
                if(random_arr[arr_index] == 2) //move up
                {
                    if(players[i].y-14*FIX-FIX >=28*FIX)
                        players[i].y -= FIX;
                }
                if(random_arr[arr_index] == 3) //move down
                {
                    if((players[i].y + FIX) <=122*FIX)
                        players[i].y += FIX;
                }
            }
            else if(i >=4) //red_players
            {
                /**   red_players moving toward the ball    **/
                if((soccerball.bally-8 > players[i].y)&&(attack == 0))
                {
                    attack = 1;
                    if((players[i].y + FIX) <=122*FIX)
                        players[i].y += FIX;
                }
                if((soccerball.bally < players[i].y-14) && (defend == 0))
                {
                    defend= 1;
                    if(players[i].y-14*FIX-FIX >=36*FIX)
                        players[i].y -= FIX;
                }
                attack --;
                defend --;
                if((soccerball.ballx+8 < players[i].x)&&(move_right == 0))
                {
                    move_right = 1;
                    if((players[i].x+10*FIX+FIX) <= 127*FIX)
                        players[i].x += FIX;
                }
                if((soccerball.ballx > players[i].x + 10)&&(move_left == 0))
                {
                    move_left = 1;
                    if(((players[i].x+10*FIX)-FIX) >= 10*FIX)
                        players[i].x -= FIX;
                }
                move_right --;
                move_left --;
                /**  Random movement   **/
                if(random_arr[arr_index] == 0) //move left
                {
                    if(((players[i].x+10*FIX)-2*FIX) >= 10*FIX)
                        players[i].x -= 2*FIX;
                }
                if(random_arr[arr_index] == 1) //move right
                {
                    if((players[i].x+10*FIX+2*FIX) <= 127*FIX)
                        players[i].x += 2*FIX;
                }
                if(random_arr[arr_index] == 2) //move up
                {
                    if(players[i].y-14*FIX-2*FIX >=36*FIX)
                        players[i].y -= 2*FIX;
                }
                if(random_arr[arr_index] == 3) //move down
                {
                    if((players[i].y + 2*FIX) <=122*FIX)
                        players[i].y += 2*FIX;
                }
            }
            arr_index++;
        }
        //red_goalkeeper toward the ball
        if(soccerball.ballx > (players[7].x+10))
        {
            if((players[7].x + FIX)<1503)
            {
                players[7].x += FIX;
            }
        }
        if((soccerball.ballx + 7) < (players[7].x))
        {
            if((players[7].x -FIX)>400)
            {
                players[7].x -= FIX;
            }
        }
        //red_goalkeeper random
        if(((random_arr[5]) % 2) == 1)
        {
            if((players[7].x + 3*FIX) < 1503)
                players[7].x += 3*FIX;
        }
        else
        {
            if((players[7].x-3*FIX) > 400)
            {
                players[7].x -= 3*FIX;
            }
        }
        /**     ball move      **/
        uint32_t distance_x, distance_y;
        soccerball.oldballx = soccerball.ballx;
        soccerball.oldbally = soccerball.bally;
        //for blue_players
        for(int i=0;i<4;i++)
        {
            //if ball near blue_players
            if((soccerball.bally >= (players[i].oldy - 15*FIX)) && (soccerball.bally <= (players[i].oldy + 8*FIX)) && (soccerball.ballx+30*FIX >= players[i].oldx+30*FIX-8*FIX) && (soccerball.ballx <= players[i].oldx+11*FIX))
            {
                if(switchpass_pressed == 1) //pass
                {
                    switchpass_pressed = 0;
                    passandshoot_cnt = 10;
                    dribble = 0;
                    if(ADCX > 2047)     //-vx
                    {
                        ball_vx = ((ConvertNegative(ADCX))<<2);
                        positive_x_flag = 0;
                        if(((soccerball.ballx + 7*FIX)-ball_vx) > 7*FIX)
                            soccerball.ballx -= ball_vx;
                    }
                    else                //+vx
                    {
                        ball_vy = ((ConvertPositive(ADCX)) << 2);
                        positive_x_flag = 1;
                        if(((soccerball.ballx + 7*FIX) + ball_vx) < 127*FIX)
                            soccerball.ballx += ball_vx;
                    }
                    if(ADCY > 2047)     //-vy
                    {
                        ball_vy = ((ConvertNegative(ADCY))<<2);
                        positive_y_flag = 0;
                        if((soccerball.bally - 7*FIX - ball_vy) > 28*FIX)
                            soccerball.bally -= ball_vy;
                    }
                    else                //+vy
                    {
                        ball_vy = ((ConvertPositive(ADCY))<<2);
                        positive_y_flag = 1;
                        if((soccerball.bally + ball_vy) < 130*FIX)
                            soccerball.bally += ball_vy;
                    }
                }
                else if(switchshoot_pressed == 1) //shoot
                {
                    switchshoot_pressed = 0;
                    passandshoot_cnt = 5;
                    dribble = 0;
                    if(i != 3)
                    {
                        if(ADCX > 2047)     //-vx
                        {
                            ball_vx = ((ConvertNegative(ADCX))<<3) ;
                            positive_x_flag = 0;
                            if(((soccerball.bally >= 10*FIX) && (soccerball.bally <= 27*FIX)) || ((soccerball.bally >= 131*FIX) && (soccerball.bally <= 156*FIX)))
                            {
                                if(((soccerball.ballx + 7*FIX)-ball_vx) > (7+55)*FIX)
                                    soccerball.ballx -= ball_vx;
                            }
                            else
                            {
                                if(((soccerball.ballx + 7*FIX)-ball_vx) > 7*FIX)
                                    soccerball.ballx -= ball_vx;
                            }
                        }
                        else                //+vx
                        {
                            ball_vy = ((ConvertPositive(ADCX)) << 3);
                            positive_x_flag = 1;
                            if(((soccerball.bally >= 10*FIX) && (soccerball.bally <= 27*FIX)) || ((soccerball.bally >= 131*FIX) && (soccerball.bally <= 156*FIX)))
                            {
                                if(((soccerball.ballx + 7*FIX) + ball_vx) < (94+7)*FIX)
                                    soccerball.ballx += ball_vx;
                            }
                            else
                            {
                                if(((soccerball.ballx + 7*FIX) + ball_vx) < 127*FIX)
                                    soccerball.ballx += ball_vx;
                            }
                        }
                        if(ADCY > 2047)     //-vy
                        {
                            ball_vy = ((ConvertNegative(ADCY))<<3);
                            positive_y_flag = 0;
                            if((soccerball.ballx >= 30*FIX) && (soccerball.ballx <= 90*FIX))
                            {
                                if((soccerball.bally - 7*FIX -ball_vy) > 3*FIX)
                                    soccerball.bally -= ball_vy;
                            }
                            else
                            {
                                if((soccerball.bally - 7*FIX - ball_vy) > 28*FIX)
                                    soccerball.bally -= ball_vy;
                            }
                        }
                        else                //+vy
                        {
                            ball_vy = ((ConvertPositive(ADCY))<<3);
                            positive_y_flag = 1;
                            if((soccerball.ballx >= 30*FIX) && (soccerball.ballx <= 90*FIX))
                            {
                                if((soccerball.bally + ball_vy) < 156*FIX)
                                    soccerball.bally += ball_vy;
                            }
                            else
                            {
                                if((soccerball.bally + ball_vy) < 130*FIX)
                                    soccerball.bally += ball_vy;
                            }
                        }
                    }
                    //blue goal_keeper
                    if(i == 3)
                    {
                        ball_vx = 0;
                        ball_vy = 4*FIX;
                        positive_y_flag = 0;
                        if((soccerball.bally - 7*FIX - ball_vy) > 28*FIX)
                            soccerball.bally -= ball_vy;
                    }
                }
                else //dribble
                {
                    dribble = 1;
                    if(soccerball.ballx > players[i].x)
                    {
                        distance_x = soccerball.ballx - players[i].oldx;
                        soccerball.ballx = players[i].x + distance_x;
                    }
                    else
                    {
                        distance_x = players[i].oldx - soccerball.ballx;
                        soccerball.ballx = players[i].x - distance_x;
                    }

                    if(soccerball.bally > players[i].y)
                    {
                        distance_y = soccerball.bally - players[i].oldy;
                        soccerball.bally = players[i].y + distance_y;
                    }
                    else
                    {
                        distance_y = players[i].oldy - soccerball.bally;
                        soccerball.bally = players[i].y - distance_y;
                    }
                }
            }
        }
        //for red_players
        for(int i=4;i<=7;i++)
        {
            if((soccerball.bally >= (players[i].oldy - 15*FIX)) && (soccerball.bally <= (players[i].oldy + 8*FIX)) && (soccerball.ballx+30*FIX >= players[i].oldx+30*FIX-8*FIX) && (soccerball.ballx <= players[i].oldx+11*FIX))
            {
                if(shoot_cnt != 3) //dribble
                {
                    red_dribble = 1;
                    shoot_cnt++;
                    if(soccerball.ballx > players[i].x)
                    {
                        distance_x = soccerball.ballx - players[i].oldx;
                        soccerball.ballx = players[i].x + distance_x;
                    }
                    else
                    {
                        distance_x = players[i].oldx - soccerball.ballx;
                        soccerball.ballx = players[i].x - distance_x;
                    }

                    if(soccerball.bally > players[i].y)
                    {
                        distance_y = soccerball.bally - players[i].oldy;
                        soccerball.bally = players[i].y + distance_y;
                    }
                    else
                    {
                        distance_y = players[i].oldy - soccerball.bally;
                        soccerball.bally = players[i].y - distance_y;
                    }
                }
                if(shoot_cnt == 3) //shoot
                {
                    shoot_cnt = 0;
                    red_dribble = 0;
                    red_passandshoot_cnt = 5;
                }
            }
        }
        if((red_dribble == 0) && (red_passandshoot_cnt > 0)) //shoot motion
        {
            red_passandshoot_cnt--;
            if((soccerball.ballx >= 30*FIX) && (soccerball.ballx <= 90*FIX))
            {
                if((soccerball.bally + 6*FIX) < 156*FIX)
                    soccerball.bally += 6*FIX;
            }
            else
            {
                red_dribble = 0;
                if((soccerball.bally + 6*FIX) < 130*FIX)
                    soccerball.bally += 6*FIX;
            }

        }
        if((dribble == 0)&& (passandshoot_cnt > 0)) //pass and shoot motion
        {
            passandshoot_cnt--;
            if(positive_x_flag == 0) //-x
            {
                if(((soccerball.bally >= 10*FIX) && (soccerball.bally <= 27*FIX)) || ((soccerball.bally >= 131*FIX) && (soccerball.bally <= 156*FIX)))
                {
                    if((soccerball.ballx + 7*FIX)-ball_vx > (7+55)*FIX)
                        soccerball.ballx -= ball_vx;
                }
                else
                {
                    if(((soccerball.ballx + 7*FIX)-ball_vx) > 7*FIX)
                        soccerball.ballx -= ball_vx;
                }
            }
            else //+x
            {
                if(((soccerball.bally >= 10*FIX) && (soccerball.bally <= 27*FIX)) || ((soccerball.bally >= 131*FIX) && (soccerball.bally <= 156*FIX)))
                {
                    if(((soccerball.ballx + 7*FIX) + ball_vx) < (94+7)*FIX)
                        soccerball.ballx += ball_vx;
                }
                else
                {
                    if(((soccerball.ballx + 7*FIX) + ball_vx) < 127*FIX)
                        soccerball.ballx += ball_vx;
                }
            }
            if(positive_y_flag == 0) //-y
            {
                if((soccerball.ballx >= 30*FIX) && (soccerball.ballx <= 90*FIX))
                {
                    if((soccerball.bally - 7*FIX -ball_vy) > 3*FIX)
                        soccerball.bally -= ball_vy;
                }
                else
                {
                    if((soccerball.bally - 7*FIX - ball_vy) > 28*FIX)
                        soccerball.bally -= ball_vy;
                }
            }
            else //+y
            {
                if((soccerball.ballx >= 30*FIX) && (soccerball.ballx <= 90*FIX))
                {
                    if((soccerball.bally + ball_vy) < 156*FIX)
                        soccerball.bally += ball_vy;
                }
                else
                {
                    if((soccerball.bally + ball_vy) < 130*FIX)
                        soccerball.bally += ball_vy;
                }
            }

        }
        //debug
        if(soccerball.ballx <= FIX)
            soccerball.ballx += FIX;

}
/*****            Reset Function               *********/
void Reset_Screen(void)
{

    ST7735_FillScreen(0x5549); //soccer field color
    ST7735_DrawBitmap(0,27,goalpost1, 128,35);
    ST7735_DrawBitmap(0,166,goalpost2, 128,35);

    players[0].x = 5*FIX; players[0].y = 100*FIX; players[0].oldx = 5*FIX; players[0].oldy = 100*FIX;
    players[1].x = 55*FIX; players[1].y = 100*FIX; players[1].oldx = 55*FIX; players[1].oldy = 100*FIX;
    players[2].x = 105*FIX; players[2].y = 100*FIX; players[2].oldx = 105*FIX; players[2].oldy = 100*FIX;
    players[3].x = 55*FIX; players[3].y = 131*FIX; players[3].oldx = 55*FIX; players[3].oldy = 131*FIX;
    players[4].x = 5*FIX; players[4].y = 60*FIX; players[4].oldx = 5*FIX; players[4].oldy = 60*FIX;
    players[5].x = 55*FIX; players[5].y = 60*FIX; players[5].oldx = 55*FIX; players[5].oldy = 60*FIX;
    players[6].x = 105*FIX; players[6].y = 60*FIX; players[6].oldx = 105*FIX; players[6].oldy = 60*FIX;
    players[7].x = 55*FIX; players[7].y = 42*FIX; players[7].oldx = 55*FIX; players[7].oldy = 42*FIX;
    soccerball.ballx = 55*FIX; soccerball.bally = 80*FIX; soccerball.oldballx = 55*FIX; soccerball.oldbally = 80*FIX;
}



//**             TimerG12 Handler              **/
void TIMG12_IRQHandler(void){uint32_t pos, msg;
  if((TIMG12->CPU_INT.IIDX) == 1){ // this will acknowledge
    GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
    GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
// game engine goes here
    // 1) sample slide pot
    for(int i=0;i<8;i++)
    {
        if((i==3) || (i == 7))
        {
            players[i].oldx = players[i].x;
        }
        else
        {
            players[i].oldx = players[i].x;
            players[i].oldy = players[i].y;
        }

    }
    Data= ADCin();
    uint32_t goalkeeper_pos = ConvertGoalkeeper(Data);
    players[3].x = goalkeeper_pos + 30*FIX;
    ADC_InDual(ADC0, &ADCY, &ADCX);
    Move(ADCX,ADCY);

    // 2) read input switches
    static uint32_t last0 = 0;
    static uint32_t last1 = 0;
    uint32_t now0 = Switch_InA();
    uint32_t now1 = Switch_InB();
    if((now0 != 0) && (last0 == 0))
    {
        switchplayer_pressed = 1;
        playernumber++;
        if(playernumber == 3)
            playernumber = 0;
    }
    last0 = now0;
    if((now1 != 0) && (last1 == 0))
    {
        if((now1 & 0x1)== 1)
            switchshoot_pressed = 1;
        if((now1 & 0x2) == 2)
            switchpass_pressed = 1;
        if((now1 & 0x08) == 8)
            switchreset_pressed = 1;
    }
    last1 = now1;
    GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
  }
}

uint8_t TExaS_LaunchPadLogicPB27PB26(void){
  return (0x80|((GPIOB->DOUT31_0>>26)&0x03));
}

typedef enum {English, Spanish, Portuguese, French} Language_t;
Language_t myLanguage=English;
typedef enum {HELLO, GOODBYE, LANGUAGE} phrase_t;
const char Hello_English[] ="Hello";
const char Hello_Spanish[] ="\xADHola!";
const char Hello_Portuguese[] = "Ol\xA0";
const char Hello_French[] ="All\x83";
const char Goodbye_English[]="Goodbye";
const char Goodbye_Spanish[]="Adi\xA2s";
const char Goodbye_Portuguese[] = "Tchau";
const char Goodbye_French[] = "Au revoir";
const char Language_English[]="English";
const char Language_Spanish[]="Espa\xA4ol";
const char Language_Portuguese[]="Portugu\x88s";
const char Language_French[]="Fran\x87" "ais";
const char *Phrases[3][4]={
  {Hello_English,Hello_Spanish,Hello_Portuguese,Hello_French},
  {Goodbye_English,Goodbye_Spanish,Goodbye_Portuguese,Goodbye_French},
  {Language_English,Language_Spanish,Language_Portuguese,Language_French}
};

void Start_Game(void)
{
        ST7735_FillScreen(0x0000); // set screen to black
        ST7735_SetCursor(2,4);
        ST7735_OutString("Hello! For English");
        ST7735_SetCursor(3,5);
        ST7735_OutString("Press Switch 1");
        ST7735_SetCursor(2,9);
        ST7735_OutString("Hola! Para Espanol");
        ST7735_SetCursor(6,10);
        ST7735_OutString("Presione");
        ST7735_SetCursor(2,11);
        ST7735_OutString("El Interruptor 2");
}
void Reset_Game(uint32_t blue_score, uint32_t red_score)
{
    ST7735_FillScreen(0x0000); // set screen to black
    if((blue_score == 3) && (chosenLang == 1))
    {
         ST7735_SetCursor(7,7);
         ST7735_OutString("You Won!");
         ST7735_SetCursor(4,8);
         ST7735_OutString("Press Switch 3");
         ST7735_SetCursor(4,9);
         ST7735_OutString("To Play Again");
    }
    else if((blue_score == 3) && (chosenLang == 0))
    {
         ST7735_SetCursor(6,6);
         ST7735_OutString("Ganarais!");
         ST7735_SetCursor(6,7);
         ST7735_OutString("Presione");
         ST7735_SetCursor(3,8);
         ST7735_OutString("El Interruptor 3");
         ST7735_SetCursor(1,9);
         ST7735_OutString("Para Jugar De Nuevo");
    }
    else if((red_score == 3) && (chosenLang == 1))
    {
         ST7735_SetCursor(6,7);
         ST7735_OutString("You Lost!");
         ST7735_SetCursor(4,8);
         ST7735_OutString("Press Switch 3");
         ST7735_SetCursor(4,9);
         ST7735_OutString("To Play Again");
    }
    else
    {
         ST7735_SetCursor(7,6);
         ST7735_OutString("Perdias!");
         ST7735_SetCursor(7,7);
         ST7735_OutString("Presione");
         ST7735_SetCursor(3,8);
         ST7735_OutString("El Interruptor 3");
         ST7735_SetCursor(1,9);
         ST7735_OutString("Para Jugar De Nuevo");
    }
}





// ALL ST7735 OUTPUT MUST OCCUR IN MAIN
int main(void){ // final main
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf();
  ST7735_FillScreen(ST7735_BLACK);
  ADCinit();     //PB18 = ADC1 channel 5, slidepot
  ADC_InitDual(ADC0, 0, 1, ADCVREF_VDDA);
  Switch_Init(); // initialize switches
  LED_Init();    // initialize LED
  Sound_Init();  // initialize sound
  TExaS_Init(0,0,&TExaS_LaunchPadLogicPB27PB26); // PB27 and PB26
    // initialize interrupts on TimerG12 at 30 Hz
  TimerG12_IntArm(80000000/30,2);
  // initialize all data structures
  __enable_irq();
  Start_Game();
  do {
      if(switchpass_pressed == 1)
          chosenLang = 0;
      if(switchshoot_pressed == 1)
          chosenLang = 1;
  } while((switchpass_pressed == 0) && (switchshoot_pressed == 0));

  Reset_Screen();
  uint32_t blue_score= 0;
  uint32_t red_score = 0;
  //In main code

  while(1){
            if((red_score <= 2) && (blue_score <=2))
            {
                LED_On((red_score<<4)|blue_score);
                SmallFont_OutVertical(red_score,114,6);
                SmallFont_OutVertical(blue_score,114,153);
                //SmallFont_OutVertical(ADCX,114,6);
                if((switchpass_pressed == 1) || (switchshoot_pressed == 1))
                {
                    Sound_Kick(); // call one of your sounds
                }
                if((soccerball.bally - 7*FIX) <= 28*FIX) //blue scores
                {
                    if(blue_score <= 2)
                    {
                        blue_score ++;
                        Sound_Score(); //output sound_score
                        Clock_Delay1ms(2000);
                        ST7735_FillScreen(ST7735_BLACK);
                        Reset_Screen();
                    }
                }
                if((soccerball.bally) >= 131*FIX) //red_scores
                {
                    if(red_score <= 2)
                    {
                        red_score++;
                        Sound_Score(); //output sound
                        Clock_Delay1ms(2000);
                        ST7735_FillScreen(ST7735_BLACK);
                        Reset_Screen();
                    }
                }
                for(int i =0;i<8;i++)
                {
                    ST7735_DrawBitmap(players[i].oldx/FIX, players[i].oldy/FIX,players[i].bimage, 11,15);
                    ST7735_DrawBitmap(players[i].x/FIX,players[i].y/FIX,players[i].image, 11,15);
                }
                ST7735_DrawBitmap(soccerball.oldballx/FIX, soccerball.oldbally/FIX,soccerball.bimage, 8,8);
                ST7735_DrawBitmap(soccerball.ballx/FIX, soccerball.bally/FIX, soccerball.image, 8,8);

            }
            else
            {
                LED_On((red_score<<4)|blue_score);
                switchreset_pressed = 0;
                Reset_Game(blue_score, red_score);
                while(switchreset_pressed == 0){}
                switchreset_pressed= 0;
                blue_score = 0; red_score = 0;
                LED_Off(1);
                Start_Game();
                do {
                    if(switchpass_pressed == 1)
                    {
                        chosenLang = 0;
                    }
                    if(switchshoot_pressed == 1)
                    {
                        chosenLang = 1;
                    }
                } while((switchpass_pressed == 0) && (switchshoot_pressed == 0));
                Reset_Screen();
            }
  }
}
