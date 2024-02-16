#include "Config.h"
// #include "TestFunc.h"
#include "Calendar.h"

void my_main_func()
{
    if (!configMode)
    {
        draw2in13CalendarVScreen();
        display.nextPage();
        delay(5000);
    }
    else
    {
        Serial.println("配置模式");
        delay(1000);
    }
}