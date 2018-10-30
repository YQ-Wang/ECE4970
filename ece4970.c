#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <time.h>
#include <stdlib.h>     /* atoi */
#include <pthread.h>
#include <semaphore.h>
#include <math.h>


#define SPI_CHANNEL         0// 0 or 1
#define SPI_SPEED     2000000    // Max speed is 3.6 MHz when VDD = 5 V

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
int LOWBOUND_Count = 0;
int HIGHBOUND_Count = 0;

int ADC_Value = 0;

int on = 1;
int BTNCount=0;

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

void B1Interrupt() 
{   
    if (on == 1)
    {
        digitalWrite(IN1, 0);
        on = 0;
        BTNCount = 0;
    }

    else if(on == 0)
    {
        digitalWrite(IN1, 1);
        on = 1;
        BTNCount = 0;
    }
}

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

    //-----------------------wiringpi GPIO interrupt setup ------
    if ( wiringPiISR (BTN1, INT_EDGE_FALLING, &B1Interrupt) < 0 ) 
    {
        printf("Not able to setup IRS\n");
        return -1;
    }
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
        LOWBOUND_Flag = 0;
        HIGHBOUND_Flag = 0;

        ADC_Value = getADCValue();
        //printf("value=%d\n", ADC_Value);

        if(ADC_Value < LOWBOUND)
        {
            LOWBOUND_Flag = 1;
            //printf("\nADC POWER\n\n");
        }

        if(ADC_Value > HIGHBOUND)
        {
            HIGHBOUND_Flag = 1;
            //printf("\nADC BOUND\n\n");
        }
        
        usleep(100000);

        sem_post(&my_semaphore1);
    }
}

void *triggerCircuit(void* ptr)
{
    // Configure the SPI
    if(wiringPiSPISetup(SPI_CHANNEL, SPI_SPEED) < 0) 
    {
        printf("wiringPiSPISetup failed\n");
        //return -1 ;
        exit(-1);
    }
          
    while(1)
    {
        usleep(10000);
        sem_wait(&my_semaphore2);

        if(LOWBOUND_Count>3)
        {
            printf("LOWBOUND_Count = %d\n", LOWBOUND_Count);
            //digitalWrite(IN1, 0);
            LOWBOUND_Count = 0;
        }

        if(HIGHBOUND_Count>3)
        {
            printf("HIGHBOUND_Count = %d\n", HIGHBOUND_Count);
            //digitalWrite(IN1, 0);
            HIGHBOUND_Count = 0;
        }
    }
}


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
    sem_init(&my_semaphore2,0,INIT_VALUE);
    
    pthread_t adcReading, circuitTrigger;
    pthread_create(&adcReading, NULL, readingADC, NULL);
    pthread_create(&circuitTrigger, NULL, triggerCircuit, NULL);

    while(1)  
    {
        sem_wait(&my_semaphore1);

        if(LOWBOUND_Flag == 0 && HIGHBOUND_Flag == 0)
        {
            printf("good\n");
        }

        if(LOWBOUND_Flag == 1)
        {
            LOWBOUND_Count++;
            //printf("LOWBOUND happened\n");
            LOWBOUND_Flag = 0;
        }

        if(HIGHBOUND_Flag == 1)
        {
            HIGHBOUND_Count++;
            //printf("HIGHBOUND happened\n");
            HIGHBOUND_Flag = 0;
        }

        sem_post(&my_semaphore2);
    }

    return 0;
}
