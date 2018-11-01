#include <stdio.h>
#include <wiringPi.h>
#include <unistd.h>

#define IN1 4

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

    while(1)
    {
        digitalWrite(IN1, 0);
        sleep(5);
        digitalWrite(IN1, 1);
        sleep(5);
    }

    return 0;
}