#include <stdio.h>
#include <wiringPi.h>

#define IN1 4
#define BTN1 19

int main(void)
{
    int rc = wiringPiSetupGpio();

    if (rc != 0) 
    {
        printf("Failed to wiringPiSetupGpio()\n");
        return 0;
    }

    int on = 1;
    pinMode (IN1, OUTPUT);
    //pinMode (BTN1, INPUT);
    digitalWrite(IN1, 1);

    for(;;）
    {
        digitalWrite(IN1, 0);
        sleep(5);
        digitalWrite(IN1, 1);
        sleep(5);
    }

    /*
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
    */

    return 0;
}