#include"mbed.h"
#include "bbcar.h"
#include "bbcar_rpc.h"
#include "stdlib.h"
#include "string.h"
#include <math.h>
PwmOut pin5(D5), pin6(D6);
Ticker servo_ticker;
BufferedSerial pc(USBTX,USBRX); //tx,rx
BufferedSerial uart(A1,A0); //tx,rx
DigitalInOut ping(D10);
BufferedSerial xbee(D1, D0);

BBCar car(pin5, pin6, servo_ticker);

Thread t1;
Timer t;
EventQueue queue(64 * EVENTS_EVENT_SIZE);
char recvall[30], tmp[20];
int now;
int re = 1;

void Follow();
void RD();
int main(){
   xbee.set_baud(9600);
   char recv[1];
   uart.set_baud(9600);
   t1.start(callback(&queue, &EventQueue::dispatch_forever));
   queue.call(Follow);

   while(1){
      if(uart.readable()){
            uart.read(recv, sizeof(recv));
            if(recv[0] == '(') now = 0;
            else if(recv[0] == ')') {
                tmp[now] = recv[0];
                if(re) strcpy(recvall, tmp);
                strcpy(tmp, "");
            }
            else if (recv[0] != ',') {
                tmp[now++] = recv[0];
            }
      } 
   }
}

void Follow(){
    char n[2][5];
    int tx, angle;
    int i = 0;
    int j = 0;
    int count = 0;
    int len;
    float a = 2.3;
    int turn = 0;
    float r;
    char buff[25];
    float dis, val;
    while(1){
        re = 0;
        i = 0;
        j = 0;
        count = 0;
        while(recvall[i] != ')'){
            if (recvall[i] == ' ') {
                j++;
                count = 0;
                i++;
            }
            n[j][count] = recvall[i];
            count++;
            i++;
        }
        tx = atoi(n[0]);
        angle = atoi(n[1]);
        len = strlen(recvall);
        re = 1;
        

        if (len == 0) {
            if (turn == 2){
                car.turn(30,1);
                ThisThread::sleep_for(75ms);
                car.stop();
            } else if (turn == 1) {
                car.turn(-30,1);
                ThisThread::sleep_for(75ms);
                car.stop();
            }
        } else if (angle > 3 && angle < 60) {
            turn = 1;
            car.goStraight(30); 
        } else if (angle < 357 && angle > 300) {
            turn = 2;
            car.goStraight(30);  
        } else { 
            if (tx <= -2){
                car.turn(30,1); 
                turn = 0;
            } else if (tx > 2) {
                car.turn(-30,1);
                turn = 0;
            } else {
                car.stop();
                ThisThread::sleep_for(500ms);
                strcpy(buff, "");
                sprintf(buff, "angle = %d\r\n", angle);
                xbee.write(buff, sizeof(buff));
                ThisThread::sleep_for(75ms);
            }
        }

        ThisThread::sleep_for(100ms);

        for (i = 0; i < 2; i++) {
            for (j = 0; j < 5; j++) {
                n[i][j] = '\0';
            }
        }
        for (i = 0; i < 30; i++) {
            recvall[i] = '\0';
        
        }
    }

}
