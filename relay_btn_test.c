#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>
#include <time.h>
#include <stdlib.h>     /* atoi */
#include <pthread.h>
#include <math.h>

#define IN1 3
#define IN2 2
#define IN3 0
#define IN4 7
#define BTN1 25

void *triggerCircuit(void* ptr)
{       
    int i;

    while(1)
    {
        usleep(10000);
        //sem_wait(&my_semaphore1);

        for(i=0;i<10;i++)
        {
            digitalWrite(IN1, 0);
            sleep(1);
            digitalWrite(IN1, 1);
            sleep(1);
        }

        /*
        if(LOWBOUND_Count>3)
        {
            //printf("LOWBOUND_Count = %d\n", LOWBOUND_Count);
            digitalWrite(IN1, 0);
            LOWBOUND_Count = 0;
        }

        if(HIGHBOUND_Count>3)
        {
            //printf("HIGHBOUND_Count = %d\n", HIGHBOUND_Count);
            digitalWrite(IN1, 0);
            HIGHBOUND_Count = 0;
        }
        */
    }
}


int main(void)
{
    if (wiringPiSetup() < 0) 
    {
        printf("Not able to setup wiringpi\n");
        return -1;
    }
          
    int on = 1;

    pinMode (IN1, OUTPUT);
    pinMode (BTN1, INPUT);
    digitalWrite(IN1, 1);

    int BTNCount=0;
    int i;
    pthread_t adcReading, circuitTrigger;
    pthread_create(&circuitTrigger, NULL, triggerCircuit, NULL);

    while(1)
    {   

        sleep(1);
    //    digitalWrite(IN1, 0);
    //    sleep(1);
    //    digitalWrite(IN1, 1);
    //     for(i=0;i<20;i++)
    //     {
    //         if(digitalRead(BTN1) == HIGH)
    //         {
    //             BTNCount++;
    //         }
    //     }

    //     if(BTNCount>16)
    //     {
    //         if (on == 1)
    //         {
    //             digitalWrite(IN1, 0);
    //             on = 0;
    //             BTNCount = 0;
    //         }

    //         else if(on == 0)
    //         {
    //             digitalWrite(IN1, 1);
    //             on = 1;
    //             BTNCount = 0;
    //         }
    //     }
    // }
    }
    return 0;
}