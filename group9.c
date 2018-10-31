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
#define CLK1 6
#define DIO1 13
#define CS1 5

#define CLK2 9
#define DIO2 11
#define CS2 10

#define CLK3 15
#define DIO3 14
#define CS3 18

// Relay
#define IN1 22
#define IN2 27
#define IN3 17
#define IN4 4

//bound
#define LOWBOUND1 10
#define HIGHBOUND1 140 
#define LOWBOUND2 10
#define HIGHBOUND2 140 
#define LOWBOUND3 10
#define HIGHBOUND3 140 

// Button
#define BTN1 26
#define BTN2 16
#define BTN3 12
#define ResetBTN 19

#define DELAYTIME 2

struct tm eventTime1_tm;
struct tm interruptTimeB1_tm;
struct timeval eventTime1;
struct timeval interruptTimeB1;
char eventTime1_string[64];
char interruptTimeB1_string[64];

struct tm eventTime2_tm;
struct tm interruptTimeB2_tm;
struct timeval eventTime2;
struct timeval interruptTimeB2;
char eventTime2_string[64];
char interruptTimeB2_string[64];

struct tm eventTime3_tm;
struct tm interruptTimeB3_tm;
struct timeval eventTime3;
struct timeval interruptTimeB3;
char eventTime3_string[64];
char interruptTimeB3_string[64];

int LOWBOUND1_Count = 0;
int HIGHBOUND1_Count = 0;
int REGULAR1_Count = 0;

int LOWBOUND1_Flag = 0;
int HIGHBOUND1_Flag = 0;

int LOWBOUND2_Count = 0;
int HIGHBOUND2_Count = 0;
int REGULAR2_Count = 0;

int LOWBOUND2_Flag = 0;
int HIGHBOUND2_Flag = 0;

int LOWBOUND3_Count = 0;
int HIGHBOUND3_Count = 0;
int REGULAR3_Count = 0;

int LOWBOUND3_Flag = 0;
int HIGHBOUND3_Flag = 0;

int ADC_Value1 = 0;
int ADC_Value2 = 0;
int ADC_Value3 = 0;

int on1 = 1;
int on2 = 1;
int on3 = 1;

int BTN1_Count = 0;
int BTN1_Flag = 0;
int BTN2_Count = 0;
int BTN2_Flag = 0;
int BTN3_Count = 0;
int BTN3_Flag = 0;

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
    
    if (on1 == 1)
    {
        digitalWrite(IN1, 1);
        gettimeofday(&interruptTimeB1, NULL);
        strftime(interruptTimeB1_string, sizeof(interruptTimeB1_string), "%Y-%m-%d %H:%M:%S", localtime_r(&interruptTimeB1.tv_sec, &interruptTimeB1_tm));
        printf("OFF: %s\n", interruptTimeB1_string);
        on1 = 0;
    }

    else if(on1 == 0)
    {
        digitalWrite(IN1, 0);
        gettimeofday(&interruptTimeB1, NULL);
        strftime(interruptTimeB1_string, sizeof(interruptTimeB1_string), "%Y-%m-%d %H:%M:%S", localtime_r(&interruptTimeB1.tv_sec, &interruptTimeB1_tm));
        printf("ON: %s\n", interruptTimeB1_string);
        on1 = 1;
    }
}

void B2Interrupt() 
{   
    BTN2_Flag = 1;

    printf("BTN2 Pressed\n");
    
    if (on2 == 1)
    {
        digitalWrite(IN2, 1);
        gettimeofday(&interruptTimeB2, NULL);
        strftime(interruptTimeB2_string, sizeof(interruptTimeB2_string), "%Y-%m-%d %H:%M:%S", localtime_r(&interruptTimeB2.tv_sec, &interruptTimeB2_tm));
        printf("OFF: %s\n", interruptTimeB2_string);
        on2 = 0;
    }

    else if(on2 == 0)
    {
        digitalWrite(IN2, 0);
        gettimeofday(&interruptTimeB2, NULL);
        strftime(interruptTimeB2_string, sizeof(interruptTimeB2_string), "%Y-%m-%d %H:%M:%S", localtime_r(&interruptTimeB2.tv_sec, &interruptTimeB2_tm));
        printf("ON: %s\n", interruptTimeB2_string);
        on2 = 1;
    }
}

void B3Interrupt() 
{   
    BTN3_Flag = 1;

    printf("BTN3 Pressed\n");
    
    if (on3 == 1)
    {
        digitalWrite(IN3, 1);
        gettimeofday(&interruptTimeB3, NULL);
        strftime(interruptTimeB3_string, sizeof(interruptTimeB3_string), "%Y-%m-%d %H:%M:%S", localtime_r(&interruptTimeB3.tv_sec, &interruptTimeB3_tm));
        printf("OFF: %s\n", interruptTimeB3_string);
        on3 = 0;
    }

    else if(on3 == 0)
    {
        digitalWrite(IN3, 0);
        gettimeofday(&interruptTimeB3, NULL);
        strftime(interruptTimeB3_string, sizeof(interruptTimeB3_string), "%Y-%m-%d %H:%M:%S", localtime_r(&interruptTimeB3.tv_sec, &interruptTimeB3_tm));
        printf("ON: %s\n", interruptTimeB3_string);
        on3 = 1;
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

    pinMode(BTN1, INPUT);
    pinMode(BTN2, INPUT);
    pinMode(BTN3, INPUT);
    pinMode(ResetBTN, INPUT);

    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);

    digitalWrite(IN1, 0);
    digitalWrite(IN2, 0);
    digitalWrite(IN3, 0);
    digitalWrite(IN4, 0);


    //----------wiringpi GPIO interrupt setup ---------
    if (wiringPiISR(BTN1, INT_EDGE_FALLING, &B1Interrupt)<0) 
    {
        printf("Not able to setup IRS\n");
        return -1;
    }
    if (wiringPiISR(BTN2, INT_EDGE_FALLING, &B2Interrupt)<0) 
    {
        printf("Not able to setup IRS\n");
        return -1;
    }
    if (wiringPiISR(BTN3, INT_EDGE_FALLING, &B3Interrupt)<0) 
    {
        printf("Not able to setup IRS\n");
        return -1;
    }
}

void *triggerCircuit1(void* ptr)
{       
    int i;

    while(1)
    {
        usleep(10000);

        if(BTN1_Flag == 0)
        {
            if(LOWBOUND1_Count>2)
            {
                //printf("LOWBOUND1_Count = %d\n", LOWBOUND1_Count);
                gettimeofday(&eventTime1, NULL);
                strftime(eventTime1_string, sizeof(eventTime1_string), "%Y-%m-%d %H:%M:%S", localtime_r(&eventTime1.tv_sec, &eventTime1_tm));
                printf("\nNO POWER: %s\n\n", eventTime1_string);
                digitalWrite(IN1, 1);
                LOWBOUND1_Count = 0;
                LOWBOUND1_Flag = 1;
            }
            
            if(HIGHBOUND1_Count>2)
            {
                //printf("HIGHBOUND1_Count = %d\n", HIGHBOUND1_Count);
                gettimeofday(&eventTime1, NULL);
                strftime(eventTime1_string, sizeof(eventTime1_string), "%Y-%m-%d %H:%M:%S", localtime_r(&eventTime1.tv_sec, &eventTime1_tm));
                printf("\nTOO HIGH: %s\n\n", eventTime1_string);
                digitalWrite(IN1, 1);
                HIGHBOUND1_Count = 0;
                HIGHBOUND1_Flag = 1;
            }
            
            if(REGULAR1_Count>20 && HIGHBOUND1_Flag == 0 && LOWBOUND1_Flag == 0)
            {
                gettimeofday(&eventTime1, NULL);
                strftime(eventTime1_string, sizeof(eventTime1_string), "%Y-%m-%d %H:%M:%S", localtime_r(&eventTime1.tv_sec, &eventTime1_tm));
                printf("GOOD: %s\n", eventTime1_string);
                digitalWrite(IN1, 0);
                REGULAR1_Count = 0;
            }

            if(HIGHBOUND1_Flag == 1 || LOWBOUND1_Flag == 1)
            {
                sleep(5);
                HIGHBOUND1_Flag = 0;
                LOWBOUND1_Flag = 0;
            }
        }
    }
}

void *triggerCircuit2(void* ptr)
{       
    int i;

    while(1)
    {
        usleep(10000);

        if(BTN2_Flag == 0)
        {
            if(LOWBOUND2_Count>2)
            {
                //printf("LOWBOUND2_Count = %d\n", LOWBOUND2_Count);
                gettimeofday(&eventTime2, NULL);
                strftime(eventTime2_string, sizeof(eventTime2_string), "%Y-%m-%d %H:%M:%S", localtime_r(&eventTime2.tv_sec, &eventTime2_tm));
                printf("\nNO POWER: %s\n\n", eventTime2_string);
                digitalWrite(IN2, 1);
                LOWBOUND2_Count = 0;
                LOWBOUND2_Flag = 1;
            }
            
            if(HIGHBOUND2_Count>2)
            {
                //printf("HIGHBOUND2_Count = %d\n", HIGHBOUND2_Count);
                gettimeofday(&eventTime2, NULL);
                strftime(eventTime2_string, sizeof(eventTime2_string), "%Y-%m-%d %H:%M:%S", localtime_r(&eventTime2.tv_sec, &eventTime2_tm));
                printf("\nTOO HIGH: %s\n\n", eventTime2_string);
                digitalWrite(IN2, 1);
                HIGHBOUND2_Count = 0;
                HIGHBOUND2_Flag = 1;
            }
            
            if(REGULAR2_Count>20 && HIGHBOUND2_Flag == 0 && LOWBOUND2_Flag == 0)
            {
                gettimeofday(&eventTime2, NULL);
                strftime(eventTime2_string, sizeof(eventTime2_string), "%Y-%m-%d %H:%M:%S", localtime_r(&eventTime2.tv_sec, &eventTime2_tm));
                printf("GOOD: %s\n", eventTime2_string);
                digitalWrite(IN2, 0);
                REGULAR2_Count = 0;
            }

            if(HIGHBOUND2_Flag == 1 || LOWBOUND2_Flag == 1)
            {
                sleep(5);
                HIGHBOUND2_Flag = 0;
                LOWBOUND2_Flag = 0;
            }
        }
    }
}

void *triggerCircuit3(void* ptr)
{       
    int i;

    while(1)
    {
        usleep(10000);

        if(BTN3_Flag == 0)
        {
            if(LOWBOUND3_Count>2)
            {
                //printf("LOWBOUND3_Count = %d\n", LOWBOUND3_Count);
                gettimeofday(&eventTime3, NULL);
                strftime(eventTime3_string, sizeof(eventTime3_string), "%Y-%m-%d %H:%M:%S", localtime_r(&eventTime3.tv_sec, &eventTime3_tm));
                printf("\nNO POWER: %s\n\n", eventTime3_string);
                digitalWrite(IN3, 1);
                LOWBOUND3_Count = 0;
                LOWBOUND3_Flag = 1;
            }
            
            if(HIGHBOUND3_Count>2)
            {
                //printf("HIGHBOUND3_Count = %d\n", HIGHBOUND3_Count);
                gettimeofday(&eventTime3, NULL);
                strftime(eventTime3_string, sizeof(eventTime3_string), "%Y-%m-%d %H:%M:%S", localtime_r(&eventTime3.tv_sec, &eventTime3_tm));
                printf("\nTOO HIGH: %s\n\n", eventTime3_string);
                digitalWrite(IN3, 1);
                HIGHBOUND3_Count = 0;
                HIGHBOUND3_Flag = 1;
            }
            
            if(REGULAR3_Count>20 && HIGHBOUND3_Flag == 0 && LOWBOUND3_Flag == 0)
            {
                gettimeofday(&eventTime3, NULL);
                strftime(eventTime3_string, sizeof(eventTime3_string), "%Y-%m-%d %H:%M:%S", localtime_r(&eventTime3.tv_sec, &eventTime3_tm));
                printf("GOOD: %s\n", eventTime3_string);
                digitalWrite(IN3, 0);
                REGULAR3_Count = 0;
            }

            if(HIGHBOUND3_Flag == 1 || LOWBOUND3_Flag == 1)
            {
                sleep(5);
                HIGHBOUND3_Flag = 0;
                LOWBOUND3_Flag = 0;
            }
        }
    }
}


int getADCValue1() 
{
    pinMode(CLK1, OUTPUT);
    pinMode(CS1, OUTPUT);
    pinMode(DIO1, OUTPUT);
    digitalWrite(CLK1, LOW);
    digitalWrite(CS1, LOW);
    digitalWrite(DIO1, HIGH); // Start bit
    delay(DELAYTIME);
    digitalWrite(CLK1, HIGH);
    delay(DELAYTIME);
    digitalWrite(CLK1, LOW);
    delay(DELAYTIME);
    digitalWrite(DIO1, HIGH); // SGL
    digitalWrite(CLK1, HIGH);
    delay(DELAYTIME);
    digitalWrite(CLK1, LOW);
    delay(DELAYTIME);
    digitalWrite(DIO1, LOW); // Choose channel 0
    digitalWrite(CLK1, HIGH);
    delay(DELAYTIME);
    pinMode(DIO1, INPUT);
    digitalWrite(CLK1, LOW);
    delay(DELAYTIME);
    digitalWrite(CLK1, HIGH); // Skip a clock
    delay(DELAYTIME);
    digitalWrite(CLK1, LOW);
    delay(DELAYTIME);

    // Read the value
    int value = 0;
    int i;

    for (i=0; i<8; i++) 
    {
        digitalWrite(CLK1, HIGH);
        int bit = digitalRead(DIO1);
        value = value << 1 | bit;
        delay(DELAYTIME);
        digitalWrite(CLK1, LOW);
        delay(DELAYTIME);
    }

    digitalWrite(CS1, HIGH);
    return value;
}

void *readingADC1(void* ptr)
{
    while(1) 
    {   
        ADC_Value1 = getADCValue1();
        //printf("value=%d\n", ADC_Value1);

        if(ADC_Value1 < LOWBOUND1)
        {
            LOWBOUND1_Count++;
        }

        if(ADC_Value1 > HIGHBOUND1)
        {
            HIGHBOUND1_Count++;
        }

        if((ADC_Value1 > LOWBOUND1) && (ADC_Value1 < HIGHBOUND1))
        {
            REGULAR1_Count++;
        }
        
        usleep(10000);
    }
}

int getADCValue2() 
{
    pinMode(CLK2, OUTPUT);
    pinMode(CS2, OUTPUT);
    pinMode(DIO2, OUTPUT);
    digitalWrite(CLK2, LOW);
    digitalWrite(CS2, LOW);
    digitalWrite(DIO2, HIGH); // Start bit
    delay(DELAYTIME);
    digitalWrite(CLK2, HIGH);
    delay(DELAYTIME);
    digitalWrite(CLK2, LOW);
    delay(DELAYTIME);
    digitalWrite(DIO2, HIGH); // SGL
    digitalWrite(CLK2, HIGH);
    delay(DELAYTIME);
    digitalWrite(CLK2, LOW);
    delay(DELAYTIME);
    digitalWrite(DIO2, LOW); // Choose channel 0
    digitalWrite(CLK2, HIGH);
    delay(DELAYTIME);
    pinMode(DIO2, INPUT);
    digitalWrite(CLK2, LOW);
    delay(DELAYTIME);
    digitalWrite(CLK2, HIGH); // Skip a clock
    delay(DELAYTIME);
    digitalWrite(CLK2, LOW);
    delay(DELAYTIME);

    // Read the value
    int value = 0;
    int i;

    for (i=0; i<8; i++) 
    {
        digitalWrite(CLK2, HIGH);
        int bit = digitalRead(DIO2);
        value = value << 1 | bit;
        delay(DELAYTIME);
        digitalWrite(CLK2, LOW);
        delay(DELAYTIME);
    }

    digitalWrite(CS2, HIGH);
    return value;
}

void *readingADC2(void* ptr)
{
    while(1) 
    {   
        ADC_Value2 = getADCValue2();
        //printf("value=%d\n", ADC_Value2);

        if(ADC_Value2 < LOWBOUND2)
        {
            LOWBOUND2_Count++;
        }

        if(ADC_Value2 > HIGHBOUND2)
        {
            HIGHBOUND2_Count++;
        }

        if((ADC_Value2 > LOWBOUND2) && (ADC_Value2 < HIGHBOUND2))
        {
            REGULAR2_Count++;
        }
        
        usleep(10000);
    }
}


int getADCValue3() 
{
    pinMode(CLK3, OUTPUT);
    pinMode(CS3, OUTPUT);
    pinMode(DIO3, OUTPUT);
    digitalWrite(CLK3, LOW);
    digitalWrite(CS3, LOW);
    digitalWrite(DIO3, HIGH); // Start bit
    delay(DELAYTIME);
    digitalWrite(CLK3, HIGH);
    delay(DELAYTIME);
    digitalWrite(CLK3, LOW);
    delay(DELAYTIME);
    digitalWrite(DIO3, HIGH); // SGL
    digitalWrite(CLK3, HIGH);
    delay(DELAYTIME);
    digitalWrite(CLK3, LOW);
    delay(DELAYTIME);
    digitalWrite(DIO3, LOW); // Choose channel 0
    digitalWrite(CLK3, HIGH);
    delay(DELAYTIME);
    pinMode(DIO3, INPUT);
    digitalWrite(CLK3, LOW);
    delay(DELAYTIME);
    digitalWrite(CLK3, HIGH); // Skip a clock
    delay(DELAYTIME);
    digitalWrite(CLK3, LOW);
    delay(DELAYTIME);

    // Read the value
    int value = 0;
    int i;

    for (i=0; i<8; i++) 
    {
        digitalWrite(CLK3, HIGH);
        int bit = digitalRead(DIO3);
        value = value << 1 | bit;
        delay(DELAYTIME);
        digitalWrite(CLK3, LOW);
        delay(DELAYTIME);
    }

    digitalWrite(CS3, HIGH);
    return value;
}

void *readingADC3(void* ptr)
{
    while(1) 
    {   
        ADC_Value3 = getADCValue3();
        //printf("value=%d\n", ADC_Value3);

        if(ADC_Value3 < LOWBOUND3)
        {
            LOWBOUND3_Count++;
        }

        if(ADC_Value3 > HIGHBOUND3)
        {
            HIGHBOUND3_Count++;
        }

        if((ADC_Value3 > LOWBOUND3) && (ADC_Value3 < HIGHBOUND3))
        {
            REGULAR3_Count++;
        }
        
        usleep(10000);
    }
}


int main(void)
{
    setupWiringPiFunction();

    mysql_connect();

    pthread_t adcReading1, circuitTrigger1;
    pthread_t adcReading2, circuitTrigger2;
    pthread_t adcReading3, circuitTrigger3;

    /*
    pthread_create(&adcReading1, NULL, readingADC1, NULL);
    usleep(1000);
    pthread_create(&circuitTrigger1, NULL, triggerCircuit1, NULL);
    */

    pthread_create(&adcReading2, NULL, readingADC2, NULL);
    usleep(1000);
    pthread_create(&circuitTrigger2, NULL, triggerCircuit2, NULL);

    /*
    pthread_create(&adcReading3, NULL, readingADC3, NULL);
    usleep(1000);
    pthread_create(&circuitTrigger3, NULL, triggerCircuit3, NULL);
    */

    while(1)
    {   
        sleep(1);
    }
    
    mysql_disconnect();
    
    return 0;
}