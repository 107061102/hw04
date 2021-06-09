#include"mbed.h"
#include "bbcar.h"
#include "bbcar_rpc.h"
#include "stdlib.h"
#include "string.h"
PwmOut pin5(D5), pin6(D6);
Ticker servo_ticker;
BufferedSerial pc(USBTX,USBRX); //tx,rx
BufferedSerial uart(A1,A0); //tx,rx

BBCar car(pin5, pin6, servo_ticker);

Thread t1;
EventQueue queue(32 * EVENTS_EVENT_SIZE);

char recvall[20], tmp[20];
int now;
int re = 1;

void Follow();

int main(){
   char recv[1];
   uart.set_baud(9600);
   t1.start(callback(&queue, &EventQueue::dispatch_forever));
   queue.call(Follow);
   while(1){
      if(uart.readable()){
            uart.read(recv, sizeof(recv));
            //pc.write(recv, sizeof(recv));
            if(recv[0] == '(') now = 0;
            else if(recv[0] == ')') {
                tmp[now] = recv[0];
                if(re) strcpy(recvall, tmp);
                strcpy(tmp, "");
            }
            else if (recv[0] != ','){
                tmp[now++] = recv[0];
            }
      } 
   }
}

void Follow(){
    //char rec[20];
    //strcpy(rec, recvall);
    char n[4][4];
    int x1, x2, y1, y2;
    int i = 0;
    int j = 0;
    int count = 0;
    int dx, dy;
    int deg;
    float a = 2.3;
    while(1){
        re = 0;
        printf("%s\n", recvall);
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
        x1 = atoi(n[0]);
        y1 = atoi(n[1]);
        x2 = atoi(n[2]);
        y2 = atoi(n[3]);
        printf("%d %d %d %d\n", x1, y1, x2, y2);
        re = 1;
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                n[i][j] = '\0';
            }
        }
        for (i = 0; i < 20; i++) {
            recvall[i] = '\0';
        }
        ThisThread::sleep_for(200ms);
    }
}

