#include "mbed.h"
#include "bbcar.h"
#include "bbcar_rpc.h"
#include <stdio.h>
#include <ctype.h>
#include <math.h>
using namespace std::chrono;

#define CENTER_BASE 1500

BufferedSerial pc(USBTX,USBRX); //tx,rx
BufferedSerial uart(D1,D0); //tx,rx

Ticker servo_ticker;
PwmOut pin5(D5), pin6(D6);

DigitalInOut pin10(D10);
DigitalOut led1(LED1);

//--------------(added)--------------

Ticker encoder_ticker;
DigitalIn encoder(D11);

Thread thread_encoder;
Thread thread_ping;

volatile int steps;
volatile int last;

Thread thread_carmove;

char c;
int i=0;
int j=0;
double angle=0.0;
double x=0.0;
double angle_radius=0.0;
int direction=0;
//--------------------------------------

BBCar car(pin5, pin6, servo_ticker);

void encoder_control() {
   int value = encoder;
   if (!last && value) steps++;
   last = value;
}

void carmove(){
    while(1){
        printf("\n11111111111: %f,%f,%d\n",angle,x,i);
        ThisThread::sleep_for(5s);
        if(x !=0){
            printf("\nangle,x,i: %f,%f,%d\n",angle,x,i);
            ThisThread::sleep_for(2s);
            //angle_radius=angle/180*3.14159;
            if(angle>0 && angle<90){
                car.turn(25,-0.05);//29
                printf("::::::111:::::::\n");
                angle_radius=(90-angle)/180*3.14159;
                for(j=0;j<600*angle_radius/3.14159;j=j+1){
                    ThisThread::sleep_for(10ms);
                }
                car.stop();
                direction=1;
            }else{
                car.turn(29,0.1);//29//25//33
                printf("::::::222:::::::\n");
                printf("::::::%f:::::::\n",600*(90-(360-angle))/3.14159);
                angle_radius=(90-(360-angle))/180*3.14159;
                //angle_radius=(360-angle)/180*3.14159;
                for(j=0;j<600*angle_radius/3.14159;j=j+1){
                    printf("%d\n",j);
                    ThisThread::sleep_for(10ms);
                }
                car.stop();
                direction=2;
            }
            ThisThread::sleep_for(5s);
            //car.goStraight(50);
            //ThisThread::sleep_for(3s);
            //car.stop();


            pc.set_baud(9600);
            encoder_ticker.attach(&encoder_control, 10ms);
            
            
            steps = 0;
            last = 0;
            car.goStraight(50);//50
            printf("encoderencoderencoder\n");
            printf("x*6.2: %f",abs(x*6.2));
            printf("x*6.2-5: %f",x*6.2-5);
            while(steps*6.5*3.14/32 < abs(abs(x*6.2)-5-1)) {//-5-3//-5-1.5
                printf("stepstepstepstepstep\n");
                ThisThread::sleep_for(10ms);
            }
            car.stop();
            ThisThread::sleep_for(5s);

            //---------------(turn to face the plane)----------------
            
            printf("turnturnturn\n");
            if(direction==1){
                //car.turn(120,0.05);//110
                car.turn(-100,-0.05);
            }else{
                //car.turn(100,-0.05);//110
                car.turn(-120,0.2);//0.05
            }
            ThisThread::sleep_for(1s);
            car.stop();

            //-------------(ping)-------------------------------

            printf("pingpingpingpingpingpingping\n");
            parallax_ping  ping1(pin10);
            ThisThread::sleep_for(2s);
            printf("ping: %f\n",(float)ping1);
            /*while(1) {
                if((float)ping1>30) led1 = 1;
                else {
                    led1 = 0;
                    car.stop();
                    ThisThread::sleep_for(3s);
                    car.turn(110,0.05);
                    ThisThread::sleep_for(1s);
                    car.stop();
                    ThisThread::sleep_for(3s);
                    car.goStraight(100);
                    ThisThread::sleep_for(2s);
                    car.stop();
                    break;
                }
                ThisThread::sleep_for(10ms);
            }*/

            return;
        }
    }
    
        

    /*parallax_ping  ping1(pin10);
   car.goStraight(50);
   while(1) {
      if((float)ping1>30) led1 = 1;
      else {
         led1 = 0;
         car.stop();
            ThisThread::sleep_for(3s);
         car.turn(110,0.05);
         ThisThread::sleep_for(1s);
        car.stop();
        ThisThread::sleep_for(3s);
        car.goStraight(100);
        ThisThread::sleep_for(2s);
        car.stop();
         break;
      }
      ThisThread::sleep_for(10ms);
   }*/
}

int main() {
    

   uart.set_baud(9600);
   thread_carmove.start(carmove);

   while(1){
       /*while(1){
         car.goStraight(200);
            ThisThread::sleep_for(3s);
            car.stop();
            ThisThread::sleep_for(1s);

            pc.set_baud(9600);
            encoder_ticker.attach(&encoder_control, 10ms);
            
            
            steps = 0;
            last = 0;
            car.goStraight(50);
            while(steps*6.5*3.14/32 < 40) {
                ThisThread::sleep_for(10ms);
            }
            car.stop();
            //ThisThread::sleep_for(10s);
            ThisThread::sleep_for(5s);
       }*/

      if(uart.readable()){
            char recv[1];
            uart.read(recv, sizeof(recv));
            pc.write(recv, sizeof(recv));
            c=recv[0];
            if(c=='a'){
                i=i+1;
            }else if(c=='x'){
                i=i+1;
            }else if(c=='.'){
                i=i+1;
            }//else if(c==':'){
            //    i=i+1;
            //}
            if(i==1 && isdigit(c) == 1){
                angle=angle*10+(((int)c)-48)*1.0;
                //printf("\nangle: %f\n",angle);
            }else if(i==2 && isdigit(c) == 1){
                angle=angle+(((int)c)-48)*0.1;
                i=i+1;
                //printf("\nangle: %f\n",angle);
            }else if(i==3 && isdigit(c) == 1){
                angle=angle+(((int)c)-48)*0.01;
                i=i+1;
                //printf("\nangle: %f\n",angle);
            }else if(i==5 && isdigit(c) == 1){
                x=x*10+(((int)c)-48)*1.0;
            }else if(i==6 && isdigit(c) == 1){
                x=x+(((int)c)-48)*0.1;
                i=i+1;
            }else if(i==7 && isdigit(c) == 1){
                x=x+(((int)c)-48)*0.01;
                i=i+1;
            }



      }
   }
}