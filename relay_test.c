#include <stdio.h>
#include <wiringPi.h>

#define IN1 3
#define IN2 2
#define IN3 0
#define IN4 7

int main(void)
{
    if (wiringPiSetup() < 0) 
    {
        printf("Not able to setup wiringpi\n");
        return -1;

        pinMode (IN1, OUTPUT);
        for(;;)
        {
            digitalWrite(IN1, 1);
            delay(500);
            digitalWrite(IN1, 0);
            delay(500);
        }
    }
    return 0;
}