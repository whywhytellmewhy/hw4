#include"mbed.h"
//#include "string.h"
#include <stdio.h>
#include "bbcar.h"
#include "bbcar_rpc.h"
#include <ctype.h>
#include <math.h>


using namespace std::chrono;

#define CENTER_BASE 1500

BufferedSerial pc(USBTX,USBRX); //tx,rx
BufferedSerial uart(D1,D0); //tx,rx

Ticker servo_ticker;
PwmOut pin5(D5), pin6(D6);
//BufferedSerial xbee(D1, D0);

//--------------(added)--------------

Ticker encoder_ticker;
DigitalIn encoder(D11);

Thread thread_encoder;
Thread thread_straight;

volatile int steps;
volatile int last;
//--------------------------------------

BBCar car(pin5, pin6, servo_ticker);

Thread thread;

char c;
char d;
char e;
//string s;

int i=0;
int j=0;

//double x_1=0.0;
//double x_2=0.0;
//double y_1=0.0;
//double y_2=0.0;
int x_1=0;
int x_2=0;
int y_1=0;
int y_2=0;
double angle=0.0;


void servo_control(int speed) {
   if (speed > 200)       speed = 200;
   else if (speed < -200) speed = -200;

   pin5 = (CENTER_BASE + speed)/20000.0f;
   pin6 = (CENTER_BASE + speed)/20000.0f;
}

void encoder_control() {
   int value = encoder;
   if (!last && value) steps++;
   last = value;
}

void straight(){
    pc.set_baud(9600);
   encoder_ticker.attach(&encoder_control, 10ms);
            
            
    steps = 0;
   last = 0;
   car.goStraight(100);
   //pin5.period_ms(20);
   //pin6.period_ms(20);

   //servo_control(100);
//printf("\nservo_controlservo_controlservo_control\n");
//ThisThread::sleep_for(3s);
//car.turn(100,-1);
   while(steps*6.5*3.14/32 < 30) {
      ThisThread::sleep_for(10ms);
   }
   car.stop();

}

void carmove(){
    while(1){
        printf("\n11111111111: %d,%d,%d,%d\n",x_1,x_2,y_1,y_2);
        ThisThread::sleep_for(5s);
        if(y_2 !=0){
            ThisThread::sleep_for(2s);
            printf("x_1: %d\n",x_1);
            printf("x_2: %d\n",x_2);
            printf("y_1: %d\n",y_1);
            printf("y_2: %d\n",y_2);
            angle=atan(abs((double)x_1-(double)x_2)/((double)y_2-(double)y_1));
            //printf("abs(x_1-x_2)/(y_2-y_1): %f\n",abs(x_1-x_2)/(y_2-y_1));
            printf("angle: %f\n",angle);
            
            if(x_1<x_2){
                car.turn(29,0.05);
                printf(":::::::%f:::::::\n",600*angle/3.14159);
                for(j=0;j<600*angle/3.14159;j=j+1){
                    ThisThread::sleep_for(10ms);
                }
                car.stop();
            }else{
                car.turn(100,-0.05);
                printf(":::c::::%f:::c::::\n",600*angle/3.14159);
                for(j=0;j<600*angle/3.14159/2;j=j+1){
                    ThisThread::sleep_for(4ms);
                }
                car.stop();
            }
            ThisThread::sleep_for(5s);
            thread_straight.start(straight);

            return;
        }
        
    }
}


int main(){
   uart.set_baud(9600);

   thread.start(carmove);

   ThisThread::sleep_for(7s);

   while(1){
       /*while(1){
         car.turn(29,0.01);
         ThisThread::sleep_for(3s);
        car.stop();
        ThisThread::sleep_for(3s);
       }*/


      if(uart.readable()){
            char recv[1];
            uart.read(recv, sizeof(recv));
            pc.write(recv, sizeof(recv));
            //printf("angle: %f\n", recv);
            //printf("%c\n", recv[7]);
            //c=recv[5];
            //d=recv[6];
            //e=recv[7];
            //printf("\nc: %c\n", c);
            //printf("\nd: %c\n", d);
            //printf("\ne: %c\n", e);
            //s=recv;
            c=recv[0];
            //printf("\nc: %c\n", c);
            //if(s[0]=='x'){
            //    printf("Yes!!!");
            //}
    //        if(c=='a'){
            //    printf("Yes!!!");
    //            i=0;
    //        }else 
            if(c=='x'){
            //    printf("No!!!");
                i=i+1;
            }else if(c=='y'){
                i=i+1;
            }else if(c=='.'){
                i=i+1;
            }else if(c==':'){
                i=i+1;
            }
            if(i==2 && isdigit(c) == 1){
                x_1=x_1*10+((int)c)-48;
            }else if(i==5 && isdigit(c) == 1){
                x_2=x_2*10+((int)c)-48;
            }else if(i==8 && isdigit(c) == 1){
                y_1=y_1*10+((int)c)-48;
            }else if(i==11 && isdigit(c) == 1){
                y_2=y_2*10+((int)c)-48;
            }



      }
   }
}