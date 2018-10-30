#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <time.h>
#include <stdlib.h>     /* atoi */
#include <pthread.h>
#include <semaphore.h>
#include <math.h>
#include <mysql/mysql.h>
#include <sys/time.h>
#include <sys/timerfd.h>

#define DATABASE_NAME		"scada"
#define DATABASE_IP         "35.192.121.176"
#define DATABASE_USERNAME	"root"
#define DATABASE_PASSWORD	"ece4970"
MYSQL *mysql1;

// ADC
#define CLK 6
#define DIO 13
#define CS 5

// Relay
#define IN1 22
#define IN2 27
#define IN3 17
#define IN4 4

//bound
#define LOWBOUND 10
#define HIGHBOUND 140 

// Button
#define BTN1 26

#define DELAYTIME 2

#define INIT_VALUE 0

struct tm eventTime_tm;
struct tm interruptTimeB1_tm;

struct timeval eventTime;
struct timeval interruptTimeB1;

char eventTime_string[64];
char interruptTimeB1_string[64];



sem_t my_semaphore1,my_semaphore2;


int LOWBOUND_Count = 0;
int HIGHBOUND_Count = 0;
int REGULAR_Count = 0;

int ADC_Value = 0;

int on = 1;

int BTN1_Count = 0;
int BTN1_Flag = 0;

void mysql_connect(void)
{
    //initialize MYSQL object for connections
	mysql1 = mysql_init(NULL);

    if(mysql1 == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(mysql1));
        return;
    }

    //Connect to the database
    if(mysql_real_connect(mysql1, DATABASE_IP, DATABASE_USERNAME, DATABASE_PASSWORD, DATABASE_NAME, 0, NULL, 0) == NULL)
    {
    	fprintf(stderr, "%s\n", mysql_error(mysql1));
    }
    else
    {
        printf("Database connection successful.\n");
    }
}

void mysql_disconnect(void)
{
    mysql_close(mysql1);
    printf( "Disconnected from database.\n");
}

void B1Interrupt() 
{   
    BTN1_Flag = 1;

    printf("BTN1 Pressed\n");
    
    if (on == 1)
    {
        digitalWrite(IN1, 0);
        gettimeofday(&interruptTimeB1, NULL);
        strftime(interruptTimeB1_string, sizeof(interruptTimeB1_string), "%Y-%m-%d %H:%M:%S", localtime_r(&interruptTimeB1.tv_sec, &interruptTimeB1_tm));
        printf("OFF: %s\n", interruptTimeB1_string);
        on = 0;
    }

    else if(on == 0)
    {
        digitalWrite(IN1, 1);
        gettimeofday(&interruptTimeB1, NULL);
        strftime(interruptTimeB1_string, sizeof(interruptTimeB1_string), "%Y-%m-%d %H:%M:%S", localtime_r(&interruptTimeB1.tv_sec, &interruptTimeB1_tm));
        printf("ON: %s\n", interruptTimeB1_string);
        on = 1;
    }
}

int setupWiringPiFunction() 
{
    int rc = wiringPiSetupGpio();

    if (rc != 0) 
    {
        printf("Failed to wiringPiSetupGpio()\n");
        return 0;
    }

    pinMode(IN1, OUTPUT);

    digitalWrite(IN1, 1);

    //----------wiringpi GPIO interrupt setup ---------
    if (wiringPiISR(BTN1, INT_EDGE_RISING, &B1Interrupt)<0) 
    {
        printf("Not able to setup IRS\n");
        return -1;
    }
}

void *triggerCircuit(void* ptr)
{       
    int i;

    while(1)
    {
        usleep(10000);
        sem_wait(&my_semaphore1);

        if(BTN1_Flag == 0)
        {
            if(LOWBOUND_Count>2)
            {
                //printf("LOWBOUND_Count = %d\n", LOWBOUND_Count);
                gettimeofday(&eventTime, NULL);
                strftime(eventTime_string, sizeof(eventTime_string), "%Y-%m-%d %H:%M:%S", localtime_r(&eventTime.tv_sec, &eventTime_tm));
                printf("NO POWER: %s\n", eventTime_string);
                digitalWrite(IN1, 0);
                LOWBOUND_Count = 0;
            }
            
            if(HIGHBOUND_Count>2)
            {
                //printf("HIGHBOUND_Count = %d\n", HIGHBOUND_Count);
                gettimeofday(&eventTime, NULL);
                strftime(eventTime_string, sizeof(eventTime_string), "%Y-%m-%d %H:%M:%S", localtime_r(&eventTime.tv_sec, &eventTime_tm));
                printf("TOO HIGH: %s\n", eventTime_string);
                digitalWrite(IN1, 0);
                HIGHBOUND_Count = 0;
            }
            
            if(REGULAR_Count>7)
            {
                digitalWrite(IN1, 1);
            }
        }
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
        ADC_Value = getADCValue();
        //printf("value=%d\n", ADC_Value);

        if(ADC_Value < LOWBOUND)
        {
            LOWBOUND_Count++;
        }

        if(ADC_Value > HIGHBOUND)
        {
            HIGHBOUND_Count++;
        }

        if((ADC_Value > LOWBOUND) && (ADC_Value < HIGHBOUND))
        {
            REGULAR_Count++;
        }
        
        usleep(10000);

        sem_post(&my_semaphore1);
    }
}


int main(void)
{
    setupWiringPiFunction();

    mysql_connect();

    sem_init(&my_semaphore1,0,INIT_VALUE);

    pthread_t adcReading, circuitTrigger;
    pthread_create(&adcReading, NULL, readingADC, NULL);
    usleep(1000);
    pthread_create(&circuitTrigger, NULL, triggerCircuit, NULL);

    while(1)
    {   
        sleep(1);
    }
    
    mysql_disconnect();
    
    return 0;
}