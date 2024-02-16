// #define DEBUG

// #include "CheckIP.h"
#include "Config.h"
#include "GB2312.h"
#include "MainFunc.h"
#include "Myimg.h"
#include "NTP.h"
#include "PreFunc.h"
#include "SmartConfig.h"
#include "myString.h"


int modeNum         = 0;
int changeMode      = 0;
int modeButtonCount = 0;
int modeButtonL     = 1;

int keepNum         = 0;
int changeKeep      = 0;
int keepButtonCount = 0;
int keepButtonL     = 1;
int changeLongKeep  = 0;
int LongKeepNum     = 0;

int ticount_0 = 0;
int ticount_1 = 0;
int ticount_2 = 0;



boolean isHibernating     = false;
boolean configMode        = false;
boolean configModeDisplay = false;
boolean cleanConfig       = false;
boolean wifiUnconfigure   = false;

extern String ip;
// extern String external_ip;
extern DNSServer dnsServer;            // 服务器对象
extern WeatherNow weatherNow;          // WeatherNow对象用于获取心知天气当天信息
extern WeatherDaily weatherDaily;      // WeatherDaily对象用于获取心知天气3天内信息
extern LifeSuggestion lifeSuggestion;  // LifeSuggestion对象用于获取心知天气生活指数
extern boolean offline_mode;

void setup()
{
    pinMode(D4, OUTPUT);
    digitalWrite(D4, LOW);
    pinMode(D5, OUTPUT);
    digitalWrite(D5, LOW);
    pinMode(modePin, INPUT_PULLUP);  // 按键设为输入模式,内部上拉
    pinMode(keepPin, INPUT_PULLUP);
    Serial.begin(115200);

    EEPROM.begin(4096);
#ifdef cleanROM
    cleanEEPROM();
#endif

    epd_pre();
    delay(10);
    wifi_configure();

    u8g2Fonts.setForegroundColor(GxEPD_BLACK);
    u8g2Fonts.setBackgroundColor(GxEPD_WHITE);  // 设置背景色

    if (!offline_mode)
    {
        initNTP();
        seniverse_pre();
        seniverse_update();
        display_str("正在获取心知天气信息，请稍后...", 5, 20);
        display.nextPage();
    }

    changeMode = 1;
}

#ifdef DEBUG
int ct = 0;
void loop()
{
    delay(5000);
    while (1)
    {
        ;
    }
}
#else
void loop()
{
    if (!configMode)
    {
        switch (modeNum)
        {
        case 0:
            draw2in13CalendarVScreen();
            delay(10);

            if (changeKeep == 1 || changeLongKeep == 1 || ticount_0 == flashtime_0)
            {
                if (isHibernating == true)
                {
                    Serial.println("Wakeup");
                    display.init();
                    display.setBorder(0x77);
                    isHibernating = false;
                }
                time_update();
                seniverse_update();
                display.fillScreen(GxEPD_WHITE);
                draw2in13CalendarVScreen();
                display.nextPage();
                delay(10);
                changeKeep = 0;
                keepNum    = 0;
            }

            if (ticount_0 == flashtime_0)
            {
                ticount_0 = 0;
            }
            else if (ticount_0 == flashtime_1)
            {
                isHibernating = true;
                Serial.println("Hibernating");
                display.hibernate();
            }

            ticount_0++;
            // Serial.println(ticount_0);

            break;

        case 1:
            delay(10);

            switch (keepNum)
            {
            case 1:
                display.fillScreen(GxEPD_WHITE);
                display.drawInvertedBitmap(0, 0, gImage_BWneko, 122, 250, GxEPD_BLACK);
                display.drawInvertedBitmap(0, 0, gImage_RWneko, 122, 250, GxEPD_RED);
                break;

            case 2:
                display.fillScreen(GxEPD_WHITE);
                display.drawInvertedBitmap(0, 0, gImage_BW1, 122, 250, GxEPD_BLACK);
                display.drawInvertedBitmap(0, 0, gImage_RW1, 122, 250, GxEPD_RED);
                break;

            case 3:
                display.fillScreen(GxEPD_WHITE);
                display.drawInvertedBitmap(0, 0, gImage_BWqgwl, 122, 250, GxEPD_BLACK);
                display.drawInvertedBitmap(0, 0, gImage_RWqgwl, 122, 250, GxEPD_RED);
                break;

            case 5:
                display.fillScreen(GxEPD_WHITE);
                display.drawInvertedBitmap(0, 0, gImage_BWyy, 122, 250, GxEPD_BLACK);
                display.drawInvertedBitmap(0, 0, gImage_RWyy, 122, 250, GxEPD_RED);
                break;

            case 4:
                display.fillScreen(GxEPD_WHITE);
                // display.drawInvertedBitmap(13, 13, repo, 96, 96, GxEPD_BLACK);
                // u8g2Fonts.setCursor(20, 140);
                // u8g2Fonts.setFont(u8g2_font_wqy16_t_gb2312a);
                // u8g2Fonts.setForegroundColor(GxEPD_RED);
                // u8g2Fonts.print("扫描二维码");
                // u8g2Fonts.setCursor(12, 160);
                // u8g2Fonts.print("获取更多信息");
                // u8g2Fonts.setForegroundColor(GxEPD_BLACK);

                display.drawInvertedBitmap(0, 0, gImage_BWyy_white, 122, 250, GxEPD_BLACK);
                display.drawInvertedBitmap(0, 0, gImage_RWyy_white, 122, 250, GxEPD_RED);
                break;

            case 0:
                display.fillScreen(GxEPD_WHITE);
                display.drawInvertedBitmap(0, 0, KFC_BW, 122, 250, GxEPD_BLACK);
                display.drawInvertedBitmap(0, 0, KFC_R, 122, 250, GxEPD_RED);
                break;


            default:
                break;
            }

            if (changeKeep == 1 || changeLongKeep == 1)
            {
                if (isHibernating == true)
                {
                    Serial.println("Wakeup");
                    display.init();
                    display.setBorder(0x77);
                    isHibernating = false;
                    delay(10);
                }
                display.nextPage();
                changeKeep = 0;
            }

            myDelayHibernate(ticount_1, flashtime_1, isHibernating);

            break;

        case 3:
            switch (keepNum)
            {
            case 0:
                display.fillScreen(GxEPD_WHITE);
                break;

            case 1:
                display.fillScreen(GxEPD_RED);
                break;

            case 2:
                display.fillScreen(GxEPD_BLACK);
                break;

            default:
                break;
            }

            if (changeKeep == 1 || changeLongKeep == 1)
            {
                if (isHibernating == true)
                {
                    Serial.println("Wakeup");
                    display.init();
                    display.setBorder(0x77);
                    isHibernating = false;
                    delay(10);
                }
                display.nextPage();
                changeKeep = 0;
            }

            myDelayHibernate(ticount_2, flashtime_2, isHibernating);

            break;

        default:
            break;
        }

        if (changeMode == 1)
        {
            display.nextPage();
            changeMode = 0;
        }

        if (changeLongKeep == 1)
        {
            // display.nextPage();
            // delay(10);
            changeLongKeep = 0;
        }
    }
    else
    {
        config_page_display();

        if (configModeDisplay == true)
        {
            display.nextPage();
            configModeDisplay = false;
        }

        if (changeKeep == 1)
        {
            Serial.println("Clean Config");
            cleanConfig = true;
            changeKeep  = 0;
            display.fillScreen(GxEPD_WHITE);
            display_str("正在清除配置，请稍后...稍后将会自动重启", 5, 20);
            display.nextPage();
            EEPROM.begin(4096);
            for (int i = 0; i < 4096; i++)
            {
                EEPROM.write(i, 0);
            }
            EEPROM.commit();
            ESP.restart();
        }
    }


    // Check Mode
    if (digitalRead(modePin) == LOW)
    {
        if (modeButtonCount < 20)
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
            if (modeButtonCount < 20)
            {
                Serial.println("Mode Button Short");
                if (modeNum >= 1)
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
                configMode        = !configMode;
                configModeDisplay = true;
                changeMode        = 1;
            }
        }
        modeButtonCount = 0;
        modeButtonL     = 1;
    }

    // Check Keep
    if (digitalRead(keepPin) == LOW)
    {
        if (keepButtonCount < 20)
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
            if (keepButtonCount < 20)
            {
                Serial.println("Keep Button Short");
                Serial.printf("KeepButtonCount: %d\r\n", keepNum);
                if (keepNum >= 5)
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
                Serial.println("Long Keep");
                // epd_pre();
                // delay(100);
                if (LongKeepNum == 2)
                {
                    LongKeepNum    = 0;
                    changeLongKeep = 1;

                    Serial.println("Long Keep reset");
                    // Serial.printf("KeepButtonCount: %d\r\n", keepButtonCount);
                }
                else
                {
                    LongKeepNum++;
                    changeLongKeep = 1;

                    Serial.println("Long Keep added");
                    // Serial.printf("KeepButtonCount: %d\r\n", keepButtonCount);
                }
            }
        }
        keepButtonCount = 0;
        keepButtonL     = 1;
    }

    // 切换模式或保持模式改变时清零计时时间
    if (changeKeep == 1 || changeLongKeep == 1 || changeMode == 1)
    {
        ticount_0 = 0;
        ticount_1 = 0;
        ticount_2 = 0;
        if (isHibernating == true)
        {
            Serial.println("Wakeup");
            display.init();
            display.setBorder(0x77);
            isHibernating = false;
            delay(10);
        }
    }

    // 设置边框
    switch (LongKeepNum)
    {
    case 0:
        display.setBorder(0x77);
        break;

    case 1:
        display.setBorder(0xB7);
        break;

    case 2:
        display.setBorder(0x37);
        break;

    default:
        break;
    }
}
#endif