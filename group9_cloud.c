#include "utilities.h"
#include "cloudsql.h"

void B1Interrupt() 
{   
    BTN1_Flag = 1;

    //printf("BTN1 Pressed\n");
    
    if (on1 == 1)
    {
        digitalWrite(IN1, 1);
        gettimeofday(&interruptTimeB1, NULL);
        strftime(interruptTimeB1_string, sizeof(interruptTimeB1_string), "%Y-%m-%d %H:%M:%S", localtime_r(&interruptTimeB1.tv_sec, &interruptTimeB1_tm));

        sprintf(B1Interrupt_query, "INSERT INTO RTU_EVENT (RTU_id, Event_Type, Power, Event_Date) values ('1', 'B1 Pressed', 'OFF', '%s')", interruptTimeB1_string); 
        if (mysql_query(mysql1, B1Interrupt_query) != 0) 
        {
            printf("sql mistake\n");
        }

        printf("\nBTN1 OFF: %s\n\n", interruptTimeB1_string);
        on1 = 0;
    }
    
    else if(on1 == 0)
    {
        digitalWrite(IN1, 0);
        gettimeofday(&interruptTimeB1, NULL);
        strftime(interruptTimeB1_string, sizeof(interruptTimeB1_string), "%Y-%m-%d %H:%M:%S", localtime_r(&interruptTimeB1.tv_sec, &interruptTimeB1_tm));
        
        sprintf(B1Interrupt_query, "INSERT INTO RTU_EVENT (RTU_id, Event_Type, Power, Event_Date) values ('1', 'B1 Pressed', 'ON', '%s')", interruptTimeB1_string); 
        if (mysql_query(mysql1, B1Interrupt_query) != 0) 
        {
            printf("sql mistake\n");
        }      
        
        printf("\nBTN1 ON: %s\n\n", interruptTimeB1_string);
        on1 = 1;
    }
}

void B2Interrupt() 
{   
    BTN2_Flag = 1;

    //printf("BTN2 Pressed\n");
    
    if (on2 == 1)
    {
        digitalWrite(IN2, 1);
        gettimeofday(&interruptTimeB2, NULL);
        strftime(interruptTimeB2_string, sizeof(interruptTimeB2_string), "%Y-%m-%d %H:%M:%S", localtime_r(&interruptTimeB2.tv_sec, &interruptTimeB2_tm));

        sprintf(B2Interrupt_query, "INSERT INTO RTU_EVENT (RTU_id, Event_Type, Power, Event_Date) values ('2', 'B2 Pressed', 'OFF', '%s')", interruptTimeB2_string); 
        if (mysql_query(mysql1, B2Interrupt_query) != 0) 
        {
            printf("sql mistake\n");
        }        

        printf("\nBTN2 OFF: %s\n\n", interruptTimeB2_string);
        on2 = 0;
    }

    else if(on2 == 0)
    {
        digitalWrite(IN2, 0);
        gettimeofday(&interruptTimeB2, NULL);
        strftime(interruptTimeB2_string, sizeof(interruptTimeB2_string), "%Y-%m-%d %H:%M:%S", localtime_r(&interruptTimeB2.tv_sec, &interruptTimeB2_tm));

        sprintf(B2Interrupt_query, "INSERT INTO RTU_EVENT (RTU_id, Event_Type, Power, Event_Date) values ('2', 'B2 Pressed', 'ON', '%s')", interruptTimeB2_string); 
        if (mysql_query(mysql1, B2Interrupt_query) != 0) 
        {
            printf("sql mistake\n");
        }                

        printf("\nBTN2 ON: %s\n\n", interruptTimeB2_string);
        on2 = 1;
    }
}

void B3Interrupt() 
{   
    BTN3_Flag = 1;

    //printf("BTN3 Pressed\n");
    
    if (on3 == 1)
    {
        digitalWrite(IN3, 1);
        gettimeofday(&interruptTimeB3, NULL);
        strftime(interruptTimeB3_string, sizeof(interruptTimeB3_string), "%Y-%m-%d %H:%M:%S", localtime_r(&interruptTimeB3.tv_sec, &interruptTimeB3_tm));

        sprintf(B3Interrupt_query, "INSERT INTO RTU_EVENT (RTU_id, Event_Type, Power, Event_Date) values ('3', 'B3 Pressed', 'OFF', '%s')", interruptTimeB3_string); 
        if (mysql_query(mysql1, B3Interrupt_query) != 0) 
        {
            printf("sql mistake\n");
        }                   

        printf("\nBTN3 OFF: %s\n\n", interruptTimeB3_string);
        on3 = 0;
    }

    else if(on3 == 0)
    {
        digitalWrite(IN3, 0);
        gettimeofday(&interruptTimeB3, NULL);
        strftime(interruptTimeB3_string, sizeof(interruptTimeB3_string), "%Y-%m-%d %H:%M:%S", localtime_r(&interruptTimeB3.tv_sec, &interruptTimeB3_tm));

        sprintf(B3Interrupt_query, "INSERT INTO RTU_EVENT (RTU_id, Event_Type, Power, Event_Date) values ('3', 'B3 Pressed', 'ON', '%s')", interruptTimeB3_string); 
        if (mysql_query(mysql1, B3Interrupt_query) != 0) 
        {
            printf("sql mistake\n");
        }

        printf("\nBTN3 ON: %s\n\n", interruptTimeB3_string);
        on3 = 1;
    }
}

void ResetBTNInterrupt() 
{   
    //printf("ResetBTN Pressed\n");
    
    gettimeofday(&interruptTimeRESET, NULL);
    strftime(interruptTimeRESET_string, sizeof(interruptTimeRESET_string), "%Y-%m-%d %H:%M:%S", localtime_r(&interruptTimeRESET.tv_sec, &interruptTimeRESET_tm));

    printf("\nResetBTN Pressed: %s\n\n", interruptTimeRESET_string);

    BTN1_Flag = 0;
    BTN2_Flag = 0;
    BTN3_Flag = 0;

    sprintf(BTNRESETInterrupt_query, "INSERT INTO RTU_EVENT (RTU_id, Event_Type, Power, Event_Date) values ('123', 'RESET BTN Pressed', 'ON', '%s')", interruptTimeRESET_string); 
    if (mysql_query(mysql1, BTNRESETInterrupt_query) != 0) 
    {
            printf("sql mistake\n");
    }

    digitalWrite(IN1, 0);
    digitalWrite(IN2, 0);
    digitalWrite(IN3, 0);
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
    digitalWrite(IN4, 1);

    //----------wiringpi GPIO interrupt setup ---------
    if (wiringPiISR(BTN1, INT_EDGE_RISING, &B1Interrupt)<0) 
    {
        printf("Not able to setup IRS\n");
        return -1;
    }
    if (wiringPiISR(BTN2, INT_EDGE_RISING, &B2Interrupt)<0) 
    {
        printf("Not able to setup IRS\n");
        return -1;
    }
    if (wiringPiISR(BTN3, INT_EDGE_RISING, &B3Interrupt)<0) 
    {
        printf("Not able to setup IRS\n");
        return -1;
    }
    if (wiringPiISR(ResetBTN, INT_EDGE_RISING, &ResetBTNInterrupt)<0) 
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
                printf("1 NO POWER: %s\n", eventTime1_string);

                sprintf(triggerCircuit1_query, "INSERT INTO RTU_EVENT (RTU_id, Event_Type, Power, Event_Date, ADC_Value) values ('1', 'NO POWER', 'OFF', '%s', '%d')", eventTime1_string, ADC_Value1); 
                if (mysql_query(mysql1, triggerCircuit1_query) != 0) 
                {
                    printf("sql mistake\n");
                }    

                digitalWrite(IN1, 1);
                LOWBOUND1_Count = 0;
                LOWBOUND1_Flag = 1;
            }
            
            if(HIGHBOUND1_Count>2)
            {
                //printf("HIGHBOUND1_Count = %d\n", HIGHBOUND1_Count);
                gettimeofday(&eventTime1, NULL);
                strftime(eventTime1_string, sizeof(eventTime1_string), "%Y-%m-%d %H:%M:%S", localtime_r(&eventTime1.tv_sec, &eventTime1_tm));
                printf("1 TOO HIGH: %s\n", eventTime1_string);

                sprintf(triggerCircuit1_query, "INSERT INTO RTU_EVENT (RTU_id, Event_Type, Power, Event_Date, ADC_Value) values ('1', 'TOO HIGH', 'OFF', '%s', '%d')", eventTime1_string, ADC_Value1); 
                if (mysql_query(mysql1, triggerCircuit1_query) != 0) 
                {
                    printf("sql mistake\n");
                }   

                digitalWrite(IN1, 1);
                HIGHBOUND1_Count = 0;
                HIGHBOUND1_Flag = 1;
            }
            
            if(REGULAR1_Count>20 && HIGHBOUND1_Flag == 0 && LOWBOUND1_Flag == 0)
            {
                gettimeofday(&eventTime1, NULL);
                strftime(eventTime1_string, sizeof(eventTime1_string), "%Y-%m-%d %H:%M:%S", localtime_r(&eventTime1.tv_sec, &eventTime1_tm));
                printf("1 GOOD: %s\n", eventTime1_string);

                sprintf(triggerCircuit1_query, "INSERT INTO RTU_EVENT (RTU_id, Event_Type, Power, Event_Date, ADC_Value) values ('1', 'GOOD', 'ON', '%s', '%d')", eventTime1_string, ADC_Value1); 
                if (mysql_query(mysql1, triggerCircuit1_query) != 0) 
                {
                    printf("sql mistake\n");
                }        

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
                printf("2 NO POWER: %s\n", eventTime2_string);

                sprintf(triggerCircuit2_query, "INSERT INTO RTU_EVENT (RTU_id, Event_Type, Power, Event_Date, ADC_Value) values ('2', 'NO POWER', 'OFF', '%s', '%d')", eventTime2_string, ADC_Value2); 
                if (mysql_query(mysql1, triggerCircuit2_query) != 0) 
                {
                    printf("sql mistake\n");
                }      

                digitalWrite(IN2, 1);
                LOWBOUND2_Count = 0;
                LOWBOUND2_Flag = 1;
            }
            
            if(HIGHBOUND2_Count>2)
            {
                //printf("HIGHBOUND2_Count = %d\n", HIGHBOUND2_Count);
                gettimeofday(&eventTime2, NULL);
                strftime(eventTime2_string, sizeof(eventTime2_string), "%Y-%m-%d %H:%M:%S", localtime_r(&eventTime2.tv_sec, &eventTime2_tm));
                printf("2 TOO HIGH: %s\n", eventTime2_string);

                sprintf(triggerCircuit2_query, "INSERT INTO RTU_EVENT (RTU_id, Event_Type, Power, Event_Date, ADC_Value) values ('2', 'TOO HIGH', 'OFF', '%s', '%d')", eventTime2_string, ADC_Value2); 
                if (mysql_query(mysql1, triggerCircuit2_query) != 0) 
                {
                    printf("sql mistake\n");
                }     

                digitalWrite(IN2, 1);
                HIGHBOUND2_Count = 0;
                HIGHBOUND2_Flag = 1;
            }
            
            if(REGULAR2_Count>20 && HIGHBOUND2_Flag == 0 && LOWBOUND2_Flag == 0)
            {
                gettimeofday(&eventTime2, NULL);
                strftime(eventTime2_string, sizeof(eventTime2_string), "%Y-%m-%d %H:%M:%S", localtime_r(&eventTime2.tv_sec, &eventTime2_tm));
                printf("2 GOOD: %s\n", eventTime2_string);

                sprintf(triggerCircuit2_query, "INSERT INTO RTU_EVENT (RTU_id, Event_Type, Power, Event_Date, ADC_Value) values ('2', 'GOOD', 'ON', '%s', '%d')", eventTime2_string, ADC_Value2); 
                if (mysql_query(mysql1, triggerCircuit2_query) != 0) 
                {
                    printf("sql mistake\n");
                }      

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
                printf("3 NO POWER: %s\n", eventTime3_string);

                sprintf(triggerCircuit3_query, "INSERT INTO RTU_EVENT (RTU_id, Event_Type, Power, Event_Date, ADC_Value) values ('3', 'NO POWER', 'OFF', '%s', '%d')", eventTime3_string, ADC_Value3); 
                if (mysql_query(mysql1, triggerCircuit3_query) != 0) 
                {
                    printf("sql mistake\n");
                }     

                digitalWrite(IN3, 1);
                LOWBOUND3_Count = 0;
                LOWBOUND3_Flag = 1;
            }
            
            if(HIGHBOUND3_Count>2)
            {
                //printf("HIGHBOUND3_Count = %d\n", HIGHBOUND3_Count);
                gettimeofday(&eventTime3, NULL);
                strftime(eventTime3_string, sizeof(eventTime3_string), "%Y-%m-%d %H:%M:%S", localtime_r(&eventTime3.tv_sec, &eventTime3_tm));
                printf("3 TOO HIGH: %s\n", eventTime3_string);

                sprintf(triggerCircuit3_query, "INSERT INTO RTU_EVENT (RTU_id, Event_Type, Power, Event_Date, ADC_Value) values ('3', 'TOO HIGH', 'OFF', '%s', '%d')", eventTime3_string, ADC_Value3); 
                if (mysql_query(mysql1, triggerCircuit3_query) != 0) 
                {
                    printf("sql mistake\n");
                }      

                digitalWrite(IN3, 1);
                HIGHBOUND3_Count = 0;
                HIGHBOUND3_Flag = 1;
            }
            
            if(REGULAR3_Count>20 && HIGHBOUND3_Flag == 0 && LOWBOUND3_Flag == 0)
            {
                gettimeofday(&eventTime3, NULL);
                strftime(eventTime3_string, sizeof(eventTime3_string), "%Y-%m-%d %H:%M:%S", localtime_r(&eventTime3.tv_sec, &eventTime3_tm));
                printf("3 GOOD: %s\n", eventTime3_string);

                sprintf(triggerCircuit3_query, "INSERT INTO RTU_EVENT (RTU_id, Event_Type, Power, Event_Date, ADC_Value) values ('3', 'GOOD', 'ON', '%s', '%d')", eventTime3_string, ADC_Value3); 
                if (mysql_query(mysql1, triggerCircuit3_query) != 0) 
                {
                    printf("sql mistake\n");
                }    

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

    pthread_create(&adcReading1, NULL, readingADC1, NULL);
    usleep(1000);
    pthread_create(&circuitTrigger1, NULL, triggerCircuit1, NULL);
    
    pthread_create(&adcReading2, NULL, readingADC2, NULL);
    usleep(1000);
    pthread_create(&circuitTrigger2, NULL, triggerCircuit2, NULL);
    
    pthread_create(&adcReading3, NULL, readingADC3, NULL);
    usleep(1000);
    pthread_create(&circuitTrigger3, NULL, triggerCircuit3, NULL);

    while(1)
    {   
        sleep(1);
    }
    
    mysql_disconnect();
    
    return 0;
}