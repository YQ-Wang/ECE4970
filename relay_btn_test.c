#include <stdio.h>
#include <wiringPi.h>

#define IN1 3
#define IN2 2
#define IN3 0
#define IN4 7
#define BTN 25

int main(void)
{
    if (wiringPiSetup() < 0) 
    {
        printf("Not able to setup wiringpi\n");
        return -1;
    }
        
    int on = 1;
    pinMode (IN1, OUTPUT);
    pinMode (BTN, INPUT);
    digitalWrite(IN1, 1);

    for(;;)
    {
        if (digitalRead(BTN1) == HIGH)
        {
            if (on == 1)
            {
                digitalWrite(IN1, 0);
                on = 0;
            }
            else if(on == 0)
            {
                digitalWrite(IN1, 1);
                on = 1;
            }
        }
    }

    return 0;
}