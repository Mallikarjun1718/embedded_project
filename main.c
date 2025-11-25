#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"

#define green 1
#define blue 2
#define red 3
#define yellow 4
#define white 5


void delay(int m){
    int i,j;
    for(i=0;i<m;i++){
        for(j=0;j<10000;j++){
            //do nothing
        }
    }
}

/*void on_or_off(int status){
    int i,j;
    if(status==1){
        /*for(j=0;j<1;j++){
            for(i=0;i<24;i++){

        }
        while(TIMER0_CTL_R ==0x01){
            for(j=15;j<16;j++){
                for(i=0;i<24;i++){
                    send_data_one();
                }
            }

            //color_led(white,255,1);
            delay(100);
            for(j=0;j<1;j++){
                for(i=0;i<24;i++){
                    send_data_zero();
                }
            }
            //color_led(white,255,0);
            delay(100);
        }
    }
    else{
        color_led(white,255,0);
    }
}*/


void Countdown_timer(void){
    //COUNTDOWN TIMER
    count=0;
    int k;
    int status=0;
    SYSCTL_RCGCTIMER_R |= (1 << 0);
    TIMER0_CTL_R = 0X00;
    TIMER0_CFG_R = 0X00;//32 BIT TIMER
    TIMER0_TAMR_R = 0X02;//PERIODIC MODE
    TIMER0_TAILR_R =0;
    TIMER0_IMR_R = 0x01;//SETTING INTERRUPTS
    color_led(white,5,15);

    while(1){

        //INITIALIZING THE TIMER
        //just small press
        if(((GPIO_PORTF_DATA_R & 0X01) ==0) & (status==0)){
            delay(30);
            color_led(green,255,15);
            status=0;
            count=0;
            k=TIMER0_TAILR_R;
            break;
        }

        //SETTING TIMER VALUE
        //Increasing-one press
        //Decreasing-long press
        if (((GPIO_PORTF_DATA_R & 0X10) ==0) & (status==0)){
            delay(50);
            if (((GPIO_PORTF_DATA_R & 0X10) ==0) & (status==0)){
                if(count>=0){
                    count--;
                    TIMER0_TAILR_R -=0xF42400;
                    color_led(white,50,count);
                }
            }
            else{
                if(count<17){
                    count++;
                    TIMER0_TAILR_R +=0xF42400;
                    color_led(white,50,count);
                }
            }
            status=1;
            delay(30);
        }

        else if (((GPIO_PORTF_DATA_R & 0X10) ==0X10) & (status==1)){
            status=0;
        }

        //EXIT
    }

    while(1){
        //ENABLING TIMER
        //just one press
        if(((GPIO_PORTF_DATA_R & 0X10) ==0) & (status==0)){
            TIMER0_CTL_R |=0x01;//ENABLING TIMER
            status=1;
            delay(30);
        }

        //DISABLING TIMER
        //just one press
        if(((GPIO_PORTF_DATA_R & 0X10) ==0) & (status==1)){
            TIMER0_CTL_R &=~(0x01);//PAUSE TIMER
            status=0;
            delay(30);
        }

        //RESETTING TIMER
        //reset same timer-one press
        //exit timer-long press
        if(((GPIO_PORTF_DATA_R & 0X01) ==0) & (status==0)){
            delay(40);
            if(((GPIO_PORTF_DATA_R & 0X01) ==0) & (status==0)){
                TIMER0_CTL_R  &=~(0x01);
                count=0;
                color_led(red,255,16);
                delay(50);
                color_led(red,255,0);
                delay(50);
                color_led(red,255,16);
                delay(50);
                color_led(red,255,0);
                break;
            }

            else{
                TIMER0_CTL_R  &=~(0x01);
                TIMER0_TAILR_R=0;
                TIMER0_TAILR_R=k;
                color_led(green,255,15);
                count=0;
                step=0;
            }
        }

        //TIMER ENDS THIS IS EXECUTED
        if ((status==1) & ((TIMER0_CTL_R & 0x01)==0X00)){
            break;
        }
    }
}




void main(){

    //PORT F CONFIGURATION
    SYSCTL_RCGC2_R   |= 0x00000020;      // ENABLE CLOCK TO GPIOF
    GPIO_PORTF_LOCK_R = 0x4C4F434B;      // UNLOCK COMMIT REGISTER
    GPIO_PORTF_CR_R   = 0x1F;            // MAKE PORTF0 CONFIGURABLE
    GPIO_PORTF_DEN_R  = 0x1F;            // SET PORTF PINS 4 PIN
    GPIO_PORTF_DIR_R  = 0x0E;            // SET PORTF4 PIN AS INPUT USER SWITCH PIN
    GPIO_PORTF_PUR_R  = 0x11;            // PORTF4 IS PULLED UP
    GPIO_PORTF_DATA_R = 0x00;            // LED OFF

    NVIC_EN0_R |= (1 << 19);

    int status=0;
    color_led(green,255,0);
    //RING LED

    while(1){
        if((GPIO_PORTF_DATA_R & 0X10) ==0  & (status==0)){
            delay(30);
            status=1;
            Countdown_timer();
            status=0;
            color_led(green,255,0);
            GPIO_PORTF_DATA_R=0X00;

        }
    }
}
