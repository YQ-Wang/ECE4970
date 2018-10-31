#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <time.h>
#include <stdlib.h>     /* atoi */
#include <pthread.h>
#include <semaphore.h>
#include <math.h>
#include <string.h>
#include <mysql/mysql.h>
#include <sys/time.h>
#include <sys/timerfd.h>

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

struct tm interruptTimeRESET_tm;
struct timeval interruptTimeRESET;
char interruptTimeRESET_string[64];

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

char *B1Interrupt_query = new char[100];
char *B2Interrupt_query = new char[100];
char *B3Interrupt_query = new char[100];
char *BTNRESETInterrupt_query = new char[100];

char *triggerCircuit1_query = new char[100];
char *triggerCircuit2_query = new char[100];
char *triggerCircuit3_query = new char[100];

#endif 