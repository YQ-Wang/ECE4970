#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>

#define IN1 3
#define IN2 2
#define IN3 0
#define IN4 7
#define BTN1 25

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

    for(;;)
    {
        int i;
        
        for(i=0;i<10;i++)
        {
            if (digitalRead(BTN1) == HIGH)
            usleep(10);
            BTNCount++;
        }

        if (BTNCount>7)
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
    }

    return 0;
}