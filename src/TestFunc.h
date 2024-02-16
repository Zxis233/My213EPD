#ifndef _TESTFUNC_H_
#define _TESTFUNC_H_
#include "Config.h"

#include "Myimg.h"

void black_loop()
{
    delay(500);
    display.setBorder(0x77);
    display.fillScreen(GxEPD_BLACK);
    display.nextPage();
    delay(5000);

    display.setBorder(0xB7);
    display.fillScreen(GxEPD_WHITE);
    display.nextPage();
    delay(5000);

    // display.setBorder(0x37);
    // display.fillScreen(GxEPD_RED);
    // // display.fillScreen(GxEPD_WHITE);
    // // display.drawInvertedBitmap(0, 125, kokona_122, 122, 122, GxEPD_RED);
    // display.nextPage();
    // delay(5000);
}

void pic_loop()
{
    delay(500);
    display.setBorder(0x77);
    display.fillScreen(GxEPD_WHITE);
    display.drawInvertedBitmap(0, 0, KFC_BW, 128, 250, GxEPD_BLACK);
    display.drawInvertedBitmap(0, 0, KFC_R, 128, 250, GxEPD_RED);
    display.nextPage();
    delay(5000);

    display.setBorder(0xB7);
    display.fillScreen(GxEPD_WHITE);
    display.drawInvertedBitmap(0, 0, kokona_122, 122, 122, GxEPD_BLACK);
    display.nextPage();
    delay(5000);

    display.setBorder(0x37);
    display.fillScreen(GxEPD_WHITE);
    display.drawInvertedBitmap(0, 125, kokona_122, 122, 122, GxEPD_RED);
    display.nextPage();
    delay(5000);
}

void mode_change_test()

{
    switch (modeNum)
    {
    case 0:

        switch (keepNum)
        {
        case 0:
            display.fillScreen(GxEPD_WHITE);
            u8g2Fonts.drawUTF8(40, 40, "Mode 1, \r\n Keep 0");
            delay(10);

            break;
        case 1:
            display.fillScreen(GxEPD_WHITE);
            u8g2Fonts.drawUTF8(40, 40, "Mode 1, \r\n Keep 1");
            delay(10);

            break;

        case 2:
            display.fillScreen(GxEPD_WHITE);
            u8g2Fonts.drawUTF8(40, 40, "Mode 1, \r\n Keep 2");
            delay(10);

            break;


        default:
            break;
        }


        if (changeMode == 1)
        {
            display.nextPage();
            changeMode = 0;
            delay(10);
        }

        if (changeKeep == 1)
        {
            display.nextPage();
            changeKeep = 0;
            delay(10);
        }

        break;

    case 1:
        display.setRotation(2);

        switch (keepNum)
        {
        case 0:
            display.setBorder(0x77);
            display.fillScreen(GxEPD_WHITE);
            display.drawInvertedBitmap(0, 0, KFC_BW, 128, 250, GxEPD_BLACK);
            display.drawInvertedBitmap(0, 0, KFC_R, 128, 250, GxEPD_RED);
            break;
        case 1:
            display.setBorder(0xB7);
            display.fillScreen(GxEPD_WHITE);
            // display.drawInvertedBitmap(0, 0, kokona_122, 122, 122, GxEPD_BLACK);
            display.drawInvertedBitmap(0, 125, smartconfig, 122, 122, GxEPD_BLACK);
            break;
        case 2:
            display.setBorder(0x37);
            display.fillScreen(GxEPD_WHITE);
            display.drawInvertedBitmap(0, 125, kokona_122, 122, 122, GxEPD_RED);
            display.drawInvertedBitmap(0, 0, smartconfig, 122, 122, GxEPD_RED);
            break;

        default:
            break;
        }

        display.setRotation(1);

        if (changeMode == 1)
        {
            display.nextPage();
            changeMode = 0;
        }

        if (changeKeep == 1)
        {
            display.nextPage();
            changeKeep = 0;
        }
        break;

    default:
        break;
    }

    // Check Mode
    if (digitalRead(modePin) == LOW)
    {
        if (modeButtonCount < 30)
        {
            digitalWrite(D5, HIGH);
        }
        else
        {
            digitalWrite(D5, LOW);
        }

        modeButtonL = 0;
        modeButtonCount++;
        delay(50);
    }
    else
    {
        digitalWrite(D5, LOW);
        if (modeButtonL == 0)
        {
            if (modeButtonCount < 30)
            {
                Serial.println("Mode Button Short");
                if (modeNum == 1)
                {
                    modeNum    = 0;
                    changeMode = 1;

                    Serial.println("Mode Pin reset");
                }
                else
                {
                    modeNum++;
                    changeMode = 1;

                    Serial.println("Mode Pin added");
                }
            }
            else
            {
                Serial.println("Mode Button Long");
                // Serial.printf("KeepButtonCount: %d\r\n", keepButtonCount);
            }
        }
        modeButtonCount = 0;
        modeButtonL     = 1;
    }

    // Check Keep
    if (digitalRead(keepPin) == LOW)
    {
        if (keepButtonCount < 30)
        {
            digitalWrite(D4, HIGH);
        }
        else
        {
            digitalWrite(D4, LOW);
        }

        keepButtonL = 0;
        keepButtonCount++;
        delay(50);
    }
    else
    {
        digitalWrite(D4, LOW);
        if (keepButtonL == 0)
        {
            if (keepButtonCount < 30)
            {
                Serial.println("Keep Button Short");
                if (keepNum == 2)
                {
                    keepNum    = 0;
                    changeKeep = 1;

                    Serial.println("keep Pin reset");
                    // Serial.printf("KeepButtonCount: %d\r\n", keepButtonCount);
                }
                else
                {
                    keepNum++;
                    changeKeep = 1;

                    Serial.println("keep Pin added");
                    // Serial.printf("KeepButtonCount: %d\r\n", keepButtonCount);
                }
            }
            else
            {
                Serial.println("Keep Button Long");
                // Serial.printf("KeepButtonCount: %d\r\n", keepButtonCount);
            }
        }
        keepButtonCount = 0;
        keepButtonL     = 1;
    }
}

void border_screen_change()

{
    switch (modeNum)
    {
    case 0:
        display.setBorder(0x77);
        switch (keepNum)
        {
        case 0:
            display.fillScreen(GxEPD_WHITE);
            delay(10);
            break;

        case 1:
            display.fillScreen(GxEPD_BLACK);
            delay(10);
            break;

        case 2:
            display.fillScreen(GxEPD_RED);
            delay(10);
            break;

        default:
            break;
        }


        if (changeMode == 1)
        {
            display.nextPage();
            changeMode = 0;
            delay(10);
        }
        if (changeKeep == 1)
        {
            display.nextPage();
            changeKeep = 0;
            delay(10);
        }
        break;


    case 1:
        display.setBorder(0x37);
        switch (keepNum)
        {
        case 0:
            display.fillScreen(GxEPD_WHITE);
            delay(10);
            break;

        case 1:
            display.fillScreen(GxEPD_BLACK);
            delay(10);
            break;

        case 2:
            display.fillScreen(GxEPD_RED);
            delay(10);
            break;

        default:
            break;
        }

        if (changeMode == 1)
        {
            display.nextPage();
            changeMode = 0;
            delay(10);
        }
        if (changeKeep == 1)
        {
            display.nextPage();
            changeKeep = 0;
            delay(10);
        }
        break;


    case 2:
        display.setBorder(0xB7);
        switch (keepNum)
        {
        case 0:
            display.fillScreen(GxEPD_WHITE);
            delay(10);
            break;

        case 1:
            display.fillScreen(GxEPD_BLACK);
            delay(10);
            break;

        case 2:
            display.fillScreen(GxEPD_RED);
            delay(10);
            break;

        default:
            break;
        }

        if (changeMode == 1)
        {
            display.nextPage();
            changeMode = 0;
            delay(10);
        }
        if (changeKeep == 1)
        {
            display.nextPage();
            changeKeep = 0;
            delay(10);
        }
        break;
    }

    // Check Mode Long
    if (digitalRead(modePin) == LOW)
    {
        if (modeButtonCount < 30)
        {
            digitalWrite(D5, HIGH);
        }
        else
        {
            digitalWrite(D5, LOW);
        }

        modeButtonL = 0;
        modeButtonCount++;
        delay(50);
    }
    else
    {
        digitalWrite(D5, LOW);
        if (modeButtonL == 0)
        {
            if (modeButtonCount < 30)
            {
                Serial.println("Mode Button Short");
                if (modeNum == 2)
                {
                    modeNum    = 0;
                    changeMode = 1;

                    Serial.println("Mode Pin reset");
                }
                else
                {
                    modeNum++;
                    changeMode = 1;

                    Serial.println("Mode Pin added");
                }
            }
            else
            {
                Serial.println("Mode Button Long");
                // Serial.printf("KeepButtonCount: %d\r\n", keepButtonCount);
            }
        }
        modeButtonCount = 0;
        modeButtonL     = 1;
    }

    // Check Keep Long
    if (digitalRead(keepPin) == LOW)
    {
        if (keepButtonCount < 30)
        {
            digitalWrite(D4, HIGH);
        }
        else
        {
            digitalWrite(D4, LOW);
        }

        keepButtonL = 0;
        keepButtonCount++;
        delay(50);
    }
    else
    {
        digitalWrite(D4, LOW);
        if (keepButtonL == 0)
        {
            if (keepButtonCount < 30)
            {
                Serial.println("Keep Button Short");
                if (keepNum == 2)
                {
                    keepNum    = 0;
                    changeKeep = 1;

                    Serial.println("keep Pin reset");
                    // Serial.printf("KeepButtonCount: %d\r\n", keepButtonCount);
                }
                else
                {
                    keepNum++;
                    changeKeep = 1;

                    Serial.println("keep Pin added");
                    // Serial.printf("KeepButtonCount: %d\r\n", keepButtonCount);
                }
            }
            else
            {
                Serial.println("Keep Button Long");
                // Serial.printf("KeepButtonCount: %d\r\n", keepButtonCount);
            }
        }
        keepButtonCount = 0;
        keepButtonL     = 1;
    }
}

#endif