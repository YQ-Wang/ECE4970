#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>
#include <time.h>
#include <stdlib.h>     /* atoi */
#include <pthread.h>
#include <semaphore.h>
#include <math.h>

// ADC
#define CLK 6
#define DIO 13
#define CS 5

// Relay
#define IN1 3
#define IN2 2
#define IN3 0
#define IN4 7

//bound
#define LOWBOUND 10
#define HIGHBOUND 140 

// Button
#define BTN1 25

#define DELAYTIME 2

#define INIT_VALUE 0

sem_t my_semaphore1,my_semaphore2;


int LOWBOUND_Flag = 0;
int HIGHBOUND_Flag = 0;

int ADC_Value = 0;

enum typeEvent
{
    BTNOFF,
    BTNON,
    ADCBOUND,//ADC out of bound
    ADCPOWER,//ADC no power
    REGULAR//Regultory 1 second update
};

struct logevent
{
    char timeBuffer [26];
    time_t rawtime;
    int RTUid;
    enum typeEvent typeEventID;
} LogEvent;  

int setupWiringPiFunction() 
{
    // sets up the wiringPi library
    if (wiringPiSetup () < 0) 
    {
        printf("Unable to setup wiringPi\n");
        return -1;
    }
    
    pinMode(IN1, OUTPUT);
    pinMode(BTN1, INPUT);
}

int getADCValue() 
{
    pinMode(CLK, OUTPUT);
    pinMode(CS, OUTPUT);
    pinMode(DIO, OUTPUT);
    digitalWrite(CLK, LOW);
    digitalWrite(CS, LOW);
    digitalWrite(DIO, HIGH); // Start bit
    delay(DELAYTIME);
    digitalWrite(CLK, HIGH);
    delay(DELAYTIME);
    digitalWrite(CLK, LOW);
    delay(DELAYTIME);
    digitalWrite(DIO, HIGH); // SGL
    digitalWrite(CLK, HIGH);
    delay(DELAYTIME);
    digitalWrite(CLK, LOW);
    delay(DELAYTIME);
    digitalWrite(DIO, LOW); // Choose channel 0
    digitalWrite(CLK, HIGH);
    delay(DELAYTIME);
    pinMode(DIO, INPUT);
    digitalWrite(CLK, LOW);
    delay(DELAYTIME);
    digitalWrite(CLK, HIGH); // Skip a clock
    delay(DELAYTIME);
    digitalWrite(CLK, LOW);
    delay(DELAYTIME);

    // Read the value
    int value = 0;
    int i;

    for (i=0; i<8; i++) 
    {
        digitalWrite(CLK, HIGH);
        int bit = digitalRead(DIO);
        value = value << 1 | bit;
        delay(DELAYTIME);
        digitalWrite(CLK, LOW);
        delay(DELAYTIME);
    }

    digitalWrite(CS, HIGH);
    return value;
}

void *readingADC(void* ptr)
{
    while(1) 
    {   
        ADC_Value = getADCValue();
        //printf("value=%d\n", ADC_Value);

        if(ADC_Value < LOWBOUND)
        {
            LOWBOUND_Flag = 1;
            //printf("\nADC POWER\n\n");
            sem_post(&my_semaphore1);
        }

        if(ADC_Value > HIGHBOUND)
        {
            HIGHBOUND_Flag = 1;
            //printf("\nADC BOUND\n\n");
            sem_post(&my_semaphore1);
        }
        
        usleep(100000);
        
    }
}
/*
void *triggerCircuit(void* ptr)
{
    if(LOWBOUND_count>3)
    {
        printf("LOWBOUND_count = %d\n", LOWBOUND_count);

    }

    if(HIGHBOUND_count>3)
    {
        printf("HIGHBOUND_count = %d\n", HIGHBOUND_count);
    
    }


}
*/

int main(int argc, char *argv[]) 
{
    if(setupWiringPiFunction() < 0 )
    {
        printf("Error setup RUT\n");
        return -1;
    }        

    int rc = wiringPiSetupGpio();

    if (rc != 0) 
    {
        printf("Failed to wiringPiSetupGpio()\n");
        return 0;
    }

    sem_init(&my_semaphore1,0,INIT_VALUE);

    pthread_t adcReading;
    //, receiveThread;
    pthread_create(&adcReading, NULL, readingADC, NULL);
    //pthread_create(&circuitTrigger, NULL, triggerCircuit, NULL);

    //printf("value=%d\n", value);
    
    while(1)  
    {
        sem_wait(&my_semaphore1);
        if(LOWBOUND_Flag == 1)
        {
            printf("LOWBOUND happened\n");
            LOWBOUND_Flag = 0;
        }

        if(HIGHBOUND_Flag == 1)
        {
            printf("HIGHBOUND happened\n");
            HIGHBOUND_Flag = 0;
        }
    }

    return 0;
}
