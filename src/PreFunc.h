#include "Config.h"
extern String authcode;
extern const String reqLocation;
extern const String reqLanguage;
extern const String reqUnit;

extern WeatherNow weatherNow;
extern WeatherDaily weatherDaily;
extern LifeSuggestion lifeSuggestion;

void epd_pre()
{
    display.init();
    display.setBorder(0x77);
    u8g2Fonts.begin(display);  // 将u8g2连接到display
    u8g2Fonts.setFont(u8g2_font_wqy16_t_gb2312);
    // display.setRotation(1);    // 设置屏幕方向为横屏
    display.setRotation(2);  // 设置屏幕方向为竖屏
    display.fillScreen(GxEPD_WHITE);
    display.firstPage();
    display.display(1);
}

void seniverse_pre()
{
    // 配置当天天气请求信息
    weatherNow.config(authcode, reqLocation, reqLanguage, reqUnit);
    // 配置天气预告请求信息
    weatherDaily.config(authcode, reqLocation, reqLanguage, reqUnit);
    // 配置生活指数
    lifeSuggestion.config(authcode, reqLocation, reqLanguage, reqUnit);
}

void seniverse_update()
{
    // 更新当天天气信息
    weatherNow.update();
    // 更新天气预告信息
    weatherDaily.update();
    // 更新生活指数
    lifeSuggestion.update();
}

void config_page_display()
{
    display.fillScreen(GxEPD_WHITE);
    u8g2Fonts.setForegroundColor(GxEPD_BLACK);
    u8g2Fonts.setFont(u8g2_font_wqy14_t_gb2312a);
    u8g2Fonts.setCursor(32, 16);
    u8g2Fonts.print("联网配置");

    u8g2Fonts.setCursor(3, 40);
    u8g2Fonts.print("WIFI名称:");
    u8g2Fonts.setCursor(6, 55);
    u8g2Fonts.print(ssid);

    u8g2Fonts.setCursor(3, 80);
    u8g2Fonts.print("WIFI密码:");
    u8g2Fonts.setCursor(6, 95);
    u8g2Fonts.print(pass);

    u8g2Fonts.setCursor(3, 120);
    ip = WiFi.localIP().toString().c_str();
    u8g2Fonts.print("局域网IP:");
    u8g2Fonts.setCursor(6, 135);
    u8g2Fonts.print(ip);

    u8g2Fonts.setCursor(3, 160);
    u8g2Fonts.print("密钥: ");
    u8g2Fonts.setCursor(6, 175);
    u8g2Fonts.print(authcode);

    u8g2Fonts.setForegroundColor(GxEPD_RED);
    u8g2Fonts.setCursor(6, 220);
    u8g2Fonts.print("按下KEEP清除配置");
    u8g2Fonts.setCursor(4, 240);
    u8g2Fonts.print("长按MODE返回先前");

    u8g2Fonts.setForegroundColor(GxEPD_BLACK);
}

void cleanEEPROM()
{
    for (int i = 0; i < 4096; i++)
    {
        EEPROM.write(i, 0);
        Serial.printf("EEPROM Clean: %d\r\n", i);
    }
    EEPROM.commit();
    Serial.println("EEPROM Cleaned!");
}


/**
 * @brief Delays the execution for a specified amount of time and hibernates the display after a certain duration.
 *
 * @param countTime The current count time.
 * @param hibernateTime The time at which the display should hibernate.
 * @param isHibernating A boolean flag indicating if the display is currently hibernating.
 * @param delayTime The delay time between each iteration (default is 10 milliseconds).
 */
void myDelayHibernate(int &countTime, int &hibernateTime, bool &isHibernating, int delayTime = 10)
{
    // if (countTime == hibernateTime)
    // {
    //     if (isHibernating == false)
    //     {
    //         countTime     = 0;
    //         isHibernating = true;
    //         Serial.println("Hibernating");
    //         display.hibernate();
    //     }
    //     else
    //     {
    //         return;
    //     }
    // }
    // countTime++;
    if (isHibernating == false)
    {
        if (countTime == hibernateTime)
        {
            Serial.println("Hibernating");
            display.hibernate();
            isHibernating = true;
        }
        else
        {
            countTime++;
            // Serial.println(countTime);
        }
    }
    delay(delayTime);
}