#include "mbed.h"
#include "bbcar.h"
#include "bbcar_rpc.h"
//------------(added)----------------------
//#include "mbed_rpc.h"
using namespace std::chrono;
//-----------------------------------

Ticker servo_ticker;
PwmOut pin5(D5), pin6(D6);
BufferedSerial xbee(D1, D0);

//--------------(added)--------------
BufferedSerial pc(USBTX, USBRX);

Ticker encoder_ticker;
DigitalIn encoder(D11);

Thread thread_encoder;

volatile int steps;
volatile int last;
//--------------------------------------

BBCar car(pin5, pin6, servo_ticker);

//-------(added)------------------
void position(Arguments *in, Reply *out);
RPCFunction rpcposition(&position, "POSITION");

void back_parking(double x,double y);

//double x, y;
double d_2;
double d_1;
double d_3;
double d_4;
char c;
//--------------------------------------

//--------(added)---------------------
/*void position (Arguments *in, Reply *out){
    x = in->getArg<double>();
    y = in->getArg<double>();
    
}*/

void encoder_control() {
   int value = encoder;
   if (!last && value) steps++;
   last = value;
}



void encoder_detect(){
    pc.set_baud(9600);
   encoder_ticker.attach(&encoder_control, 10ms);
   steps = 0;
   last = 0;
   printf("encoder_detect!!!\n");
   if(c=='w'){
    d_3=(-1)*d_2*20/3-12+d_2;//-13
   }else{
       d_3=(d_2+15)*20/3+12+d_2;
   }
   car.goStraight(d_3);
   //car.servo0.set_speed(50);
   //car.servo1.set_speed(-50);


//   while(steps*6.5*3.14/32 < 100*d_2/17) {
      // printf("encoder = %d\r\n", steps);
      //ThisThread::sleep_for(100ms);
      //printf("%f\n",steps*6.5*3.14/32);
//      ThisThread::sleep_for(10ms);
//   }
//   printf("stop\n");
   //car.stop();
   ThisThread::sleep_for(5s);
//   printf("d_2: %f\n",100*d_2/17);
   //car.turn(-125,-0.1);
   if(c=='w'){
    car.turn(-110,-0.05);//125//120//110//100
   }else{
       car.turn(-110,0.05);
   }
   ThisThread::sleep_for(1s);
   car.stop();
   ThisThread::sleep_for(4s);
   steps = 0;
   last = 0;
   d_4=(-1)*(d_1+4)*20/3-12;//-13
   car.goStraight(-30);
   //ThisThread::sleep_for(5s);
   printf("d_4: %f\n",d_4);
   while(steps*6.5*3.14/32 < d_1+5) {//d_1+4
      // printf("encoder = %d\r\n", steps);
      //ThisThread::sleep_for(100ms);
      //printf("%f\n",steps*6.5*3.14/32);
      ThisThread::sleep_for(10ms);
   }
   car.stop();

}

void back_parking(double x,double y){
    d_1=0.0938*x*x-1.6875*x+16.219;//x-2.5
    d_2=y+1;
    
    printf("back_parking!!!\n");
    printf("d_2: %f\n",d_2);
    thread_encoder.start(encoder_detect);

}


void position(Arguments *in, Reply *out){
    double x = in->getArg<double>();
    double y = in->getArg<double>();
    c = in->getArg<char>();
    //car.back_for_a_distance(x,y);
    printf("position!!!\n");
    back_parking(x,y);
    return;
}



//----------------------------------



int main() {

    //-----(added)-----------------
    //thread_encoder.start(encoder_detect);
    printf("position!!!\n");
   //--------------------------------
   char buf[256], outbuf[256];
   FILE *devin = fdopen(&xbee, "r");
   FILE *devout = fdopen(&xbee, "w");
   while (1) {
      memset(buf, 0, 256);
      for( int i = 0; ; i++ ) {
         char recv = fgetc(devin);
         if(recv == '\n') {
            printf("\r\n");
            break;
         }
         buf[i] = fputc(recv, devout);
      }
   RPC::call(buf, outbuf);
   }
}