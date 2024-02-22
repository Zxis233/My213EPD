#include "Config.h"
#include "myString.h"

void drawCalendar(int startX, int startY, int calendarWidth, int calendarHeight);
int parseIconicCheck(String str);
int calStartAt(int areaMaxLen, int contentLen);

extern WeatherNow weatherNow;          // WeatherNow对象用于获取心知天气当天信息
extern WeatherDaily weatherDaily;      // WeatherDaily对象用于获取心知天气3天内信息
extern LifeSuggestion lifeSuggestion;  // LifeSuggestion对象用于获取心知天气生活指数


void draw2in13CalendarVScreen()
{
    display.setFullWindow();
    display.fillScreen(GxEPD_WHITE);  // 清屏成白色

    // x坐标绘制起点
    int startX          = 0;
    // y坐标绘制起点
    int startY          = 0;
    // 绘制内容宽度
    int contentWidth    = 0;
    // 绘制内容高度
    int contentHeight   = 0;
    // 状态栏宽度按
    int statusBarWidth  = display.height();
    // 状态栏高度
    int statusBarHeight = 0;
    // 整体 - 左 宽度
    int mainLeftWidth   = display.width();
    // 整体 - 中 宽度
    int mainCenterWidth = 15;
    // 整体 - 右 宽度
    int mainRightWidth  = 0;
    // 整体 - 高度
    int mainAllHeight   = display.height() - statusBarHeight;
    // 设置天气图标统一的宽度
    // int openIconicWeatherWidth = 60;
    // 临时变量
    char buff[100];
    String lastupdate;

    // ===== 左侧日历 =====
    // 上：年、月、星期，温度、湿度，高度占比：13*3=39
    // 下：公历日历
    int currentYear  = year();
    int currentMonth = month();
    // int currentYear  = 2023;
    // int currentMonth = 10;
    // 年
    contentHeight    = 20;
    startY           = 122;
    mainLeftWidth    = 120;
    display.fillRect(2, startY, mainLeftWidth - 2, contentHeight, GxEPD_RED);
    u8g2Fonts.setFont(u8g2_font_wqy14_t_gb2312a);
    sprintf(buff, "%d年%d月", currentYear, currentMonth);
    contentWidth  = u8g2Fonts.getUTF8Width(buff);
    startX        = calStartAt(mainLeftWidth, contentWidth);
    startY       += contentHeight - 5;
    u8g2Fonts.setForegroundColor(GxEPD_WHITE);  // 设置前景色
    u8g2Fonts.setBackgroundColor(GxEPD_RED);    // 设置背景色
    u8g2Fonts.drawUTF8(startX, startY, buff);
    u8g2Fonts.setForegroundColor(GxEPD_BLACK);  // 设置前景色
    u8g2Fonts.setBackgroundColor(GxEPD_WHITE);  // 设置背景色
    // 显示日历
    startX         = 3;
    startY        += 5;
    contentHeight  = 80;
    drawCalendar(startX, startY, mainLeftWidth, contentHeight);

    // ===== 实时天气及预报 =====
    // 线框分界线 =====
    startX       = 4;
    contentWidth = 114;
    startY       = (250 - 120) * 3 / 5 - 4;
    display.drawFastHLine(startX, startY, contentWidth, GxEPD_RED);
    display.drawFastHLine(startX, startY + 1, contentWidth, GxEPD_RED);
    startX  = 61;
    startY += 7;
    display.drawFastVLine(startX, startY, 36, GxEPD_RED);


    // 天气实况 =====
    // 天气图标
    int iconIdx   = weatherCode[weatherNow.getWeatherCode()];
    startX        = 4;
    contentHeight = 8 * 6;
    startY        = 3 + contentHeight;
    u8g2Fonts.setFont(u8g2_font_open_iconic_weather_6x_t);
    // u8g2Fonts.setForegroundColor(GxEPD_RED);
    u8g2Fonts.drawGlyph(startX + 1, startY, iconicWeather[iconIdx]);
    // u8g2Fonts.setForegroundColor(GxEPD_BLACK);
    // 天气文本描述
    contentHeight  = 15;
    startY        += contentHeight + 3;
    // 最长的天气现象描述 "今天:雷阵雨伴有冰雹"
    u8g2Fonts.setFont(u8g2_font_wqy14_t_gb2312a);
    sprintf(buff, "%s%s, %d~", ldayCN2[0], weatherNow.getWeatherText(), weatherDaily.getLow(0));
    // u8g2Fonts.drawUTF8(startX, startY, buff);
    u8g2Fonts.setCursor(startX + 2, startY);
    u8g2Fonts.print(buff);
    u8g2Fonts.setForegroundColor(GxEPD_RED);
    sprintf(buff, "%d°", weatherDaily.getHigh(0));
    u8g2Fonts.print(buff);
    u8g2Fonts.setForegroundColor(GxEPD_BLACK);
    u8g2Fonts.print("c");


    // 城市
    u8g2Fonts.setFont(u8g2_font_wqy12_t_gb2312a);
    startX     += 8 * 6 + 5 + 2;
    startY      = 13;
    lastupdate  = weatherNow.getLastUpdate();
    sprintf(buff, "%s|%s",
            weatherNow.getCityName(),
            lastupdate.substring(lastupdate.indexOf("T") + 1, lastupdate.indexOf("+") - 3).c_str());
    u8g2Fonts.drawUTF8(startX + 1, startY, buff);

    // 温度、湿度


    startY += 13;
    // sprintf(buff, "%d°c,%d%%", weatherNow.getDegree(), weatherDaily.getHumidity(0));
    // u8g2Fonts.drawUTF8(startX, startY, buff);
    u8g2Fonts.setCursor(startX, startY);
    u8g2Fonts.setForegroundColor(GxEPD_RED);
    u8g2Fonts.setFont(u8g2_font_open_iconic_thing_1x_t);
    u8g2Fonts.drawGlyph(startX, startY, 76);
    u8g2Fonts.drawGlyph(startX + 32, startY, 72);

    u8g2Fonts.setForegroundColor(GxEPD_BLACK);

    u8g2Fonts.setFont(u8g2_font_wqy12_t_gb2312a);
    sprintf(buff, "%d°c", weatherNow.getDegree());
    u8g2Fonts.drawUTF8(startX + 8, startY, buff);
    sprintf(buff, "%d%%", weatherDaily.getHumidity(0));
    u8g2Fonts.drawUTF8(startX + 8 + 32, startY, buff);


    //  生活指数
    // lifeSuggestion.getDressing() // 获取穿衣建议
    startY += 15;
    sprintf(buff, "紫外线%s", lifeSuggestion.getUV());
    u8g2Fonts.drawUTF8(startX, startY, buff);
    startY += 13;
    sprintf(buff, "%s感冒", lifeSuggestion.getFactorFlu());
    u8g2Fonts.drawUTF8(startX, startY + 1, buff);

    // 天气预报 =====
    startX = 3;
    for (int i = 1; i < 3; i++)
    {
        startX += 60 * (i - 1);
        startY  = (250 - 120) * 3 / 5 + 12;
        u8g2Fonts.setFont(u8g2_font_wqy12_t_gb2312a);
        // sprintf(buff, "%s%d~%d°", ldayCN2[i], weatherDaily.getLow(i), weatherDaily.getHigh(i));
        // u8g2Fonts.drawUTF8(startX, startY, buff);

        sprintf(buff, "%s%d~", ldayCN2[i], weatherDaily.getLow(i));
        // u8g2Fonts.drawUTF8(startX, startY, buff);
        u8g2Fonts.setCursor(startX, startY);
        u8g2Fonts.print(buff);
        u8g2Fonts.setForegroundColor(GxEPD_RED);
        sprintf(buff, "%d°", weatherDaily.getHigh(i));
        // u8g2Fonts.drawUTF8(startX, startY, buff);
        u8g2Fonts.print(buff);
        u8g2Fonts.setForegroundColor(GxEPD_BLACK);

        // 天气图标
        u8g2Fonts.setFont(u8g2_font_open_iconic_weather_1x_t);
        iconIdx  = weatherCode[weatherDaily.getDayCode(i)];
        startY  += 13;
        u8g2Fonts.drawGlyph(startX, startY, iconicWeather[iconIdx]);
        iconIdx  = weatherCode[weatherDaily.getNightCode(i)];
        startY  += 13;
        u8g2Fonts.drawGlyph(startX, startY, iconicWeather[iconIdx]);
        // 天气描述
        u8g2Fonts.setFont(u8g2_font_wqy12_t_gb2312a);
        startX += 8 * 1 + 1;
        startY -= 13;
        sprintf(buff, "日间%s", weatherDaily.getDayText(i));
        u8g2Fonts.drawUTF8(startX, startY, buff);
        startX -= 8 * 1;
        startY += 13;
        sprintf(buff, "晚间%s", weatherDaily.getNightText(i));
        u8g2Fonts.drawUTF8(startX + 8 * 1, startY, buff);
    }

    //  生活指数 =====
    // lifeSuggestion.getDressing() // 获取穿衣建议
    // lifeSuggestion.getExercise() // 获取运动建议
    u8g2Fonts.setFont(u8g2_font_open_iconic_check_1x_t);
    startX = 4;
    startY = display.height() - 4;
    u8g2Fonts.drawGlyph(startX, startY, parseIconicCheck(lifeSuggestion.getExercise()));
    // u8g2Fonts.drawGlyph(startX, startY, 67);
    u8g2Fonts.setFont(u8g2_font_wqy12_t_gb2312a);
    startX += 10;
    sprintf(buff, "运动 ");
    u8g2Fonts.drawUTF8(startX, startY, buff);
    contentWidth = u8g2Fonts.getUTF8Width(buff);
    // lifeSuggestion.getTravel() // 获取旅游建议
    u8g2Fonts.setFont(u8g2_font_open_iconic_check_1x_t);
    startX += contentWidth;
    u8g2Fonts.drawGlyph(startX, startY, parseIconicCheck(lifeSuggestion.getTravel()));
    // u8g2Fonts.drawGlyph(startX, startY, 68);
    u8g2Fonts.setFont(u8g2_font_wqy12_t_gb2312a);
    startX += 10;
    sprintf(buff, "旅游 ");
    u8g2Fonts.drawUTF8(startX, startY, buff);
    contentWidth = u8g2Fonts.getUTF8Width(buff);
    // lifeSuggestion.getCarWash() // 获取洗车建议
    u8g2Fonts.setFont(u8g2_font_open_iconic_check_1x_t);
    startX += contentWidth;
    u8g2Fonts.drawGlyph(startX, startY, parseIconicCheck(lifeSuggestion.getCarWash()));
    // u8g2Fonts.drawGlyph(startX, startY, 64);
    u8g2Fonts.setFont(u8g2_font_wqy12_t_gb2312a);
    startX += 10;
    sprintf(buff, "洗车");
    u8g2Fonts.drawUTF8(startX, startY, buff);
}



/**
 * @brief 绘制日历
 * @param calendarWidth 日历x坐标绘制起点
 * @param calendarHeight 日历y坐标绘制起点
 * @param calendarWidth 日历显示的宽度
 * @param calendarHeight 日历显示的高度
 */
void drawCalendar(int startX, int startY, int calendarWidth, int calendarHeight)
{
    String tmpStr        = "";                 // 字符串临时变量
    int contentWidth     = 0;
    int splitWidth       = calendarWidth / 7;  // 绘制内容宽度
    int splitHeight      = 18;
    int firstWeekDay     = 0;                  // 获取本地实时时间
    int currentYear      = year();
    int currentWeekDay   = weekday() - 1;
    int currentMonth     = month();
    int currentMonthDay  = day();
    int currentMonthDays = 30;


    // int firstWeekDay     = 0;
    // int currentYear      = 2023;
    // int currentWeekDay   = 3 - 1;
    // int currentMonth     = 10;
    // int currentMonthDay  = 10;
    // int currentMonthDays = 30;


    if (currentMonth == 2)
    {                                                     // 闰年2月和平年2月份天数不一样
        currentMonthDays = ((currentYear % 400 == 0) || (currentYear % 4 == 0 && currentYear % 100 > 0)) ? 29 : 28;
    }
    else if ((currentMonth < 8 && currentMonth % 2 == 1)  // 判断31天的月份
             || (currentMonth > 7 && currentMonth % 2 == 0))
    {
        currentMonthDays = 31;
    }
    // Serial.printf("%d, %d, %d, %d\r\n", currentWeekDay, currentMonthDay, currentMonthDays, currentMonthDay % 7);
    // 算当月第1天是周几
    int remainder = currentWeekDay - currentMonthDay % 7 + 1;
    firstWeekDay  = remainder < 0 ? 7 + remainder : remainder;
    // 算当月日历总行数
    int rowCount  = (firstWeekDay + currentMonthDays) % 7 == 0 ? (firstWeekDay + currentMonthDays) / 7 : (firstWeekDay + currentMonthDays) / 7 + 1;
    int colCount  = sizeof(wdayCN1) / sizeof(wdayCN1[0]);
    // 星期行
    u8g2Fonts.setFont(u8g2_font_wqy12_t_gb2312a);
    int fontWidth = u8g2Fonts.getUTF8Width(wdayCN1[0]);
    startX        = calStartAt(splitWidth, fontWidth);
    // startY = screenHeight * 3 / 7;
    for (int i = 0; i < colCount; i++)
    {
        // u8g2Fonts.drawUTF8(splitWidth * i + startX, (splitHeight - 5) + startY, wdayCN1[i]);
        if (i == 0)
        {
            u8g2Fonts.setForegroundColor(GxEPD_RED);
            u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
            u8g2Fonts.drawUTF8(splitWidth * i + startX, (splitHeight - 5) + startY, wdayCN1[i]);
        }
        else
        {
            u8g2Fonts.setForegroundColor(GxEPD_BLACK);
            u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
            u8g2Fonts.drawUTF8(splitWidth * i + startX, (splitHeight - 5) + startY, wdayCN1[i]);
        }
    }
    // 日历行
    int monthDay  = 0;
    startY       += splitHeight + 14 - 2;
    u8g2Fonts.setFont(u8g2_font_profont11_tn);
    for (int i = 0; i < rowCount; i++)
    {
        for (int j = 0; j < colCount; j++)
        {
            // 跳过本月第1天之前的天数
            if (monthDay < 1 && firstWeekDay != j)
            {
                continue;
            }
            monthDay += 1;
            // 超出本月最后一天
            if (monthDay > currentMonthDays)
            {
                break;
            }
            // 在本月天数中
            splitHeight = 14;
            tmpStr.concat(monthDay);
            startX = calStartAt(splitWidth, u8g2Fonts.getUTF8Width(tmpStr.c_str()));
            if (monthDay == currentMonthDay)
            {  // 当天要突出显示
                // display.drawRect(splitWidth * j, splitHeight * (i - 1) + startY + 5, splitWidth, splitHeight - 2, display.epd2.hasColor ? GxEPD_RED : GxEPD_BLACK);
                display.fillRect(splitWidth * j + 1, splitHeight * (i - 1) + startY + 5, splitWidth - 2, splitHeight - 3, display.epd2.hasColor ? GxEPD_RED : GxEPD_BLACK);
                if (display.epd2.hasColor)
                {
                    // u8g2Fonts.setForegroundColor(GxEPD_RED);
                    // u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
                    u8g2Fonts.setForegroundColor(GxEPD_WHITE);
                    u8g2Fonts.setBackgroundColor(GxEPD_RED);
                }
                u8g2Fonts.drawUTF8(splitWidth * j + startX, splitHeight * i + startY, tmpStr.c_str());
                if (display.epd2.hasColor)
                {
                    u8g2Fonts.setForegroundColor(GxEPD_BLACK);
                    u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
                }
            }
            else
            {
                if (j == 0)
                {
                    u8g2Fonts.setForegroundColor(GxEPD_RED);
                    u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
                    u8g2Fonts.drawUTF8(splitWidth * j + startX, splitHeight * i + startY, tmpStr.c_str());
                }
                else
                {
                    u8g2Fonts.setForegroundColor(GxEPD_BLACK);
                    u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
                    u8g2Fonts.drawUTF8(splitWidth * j + startX, splitHeight * i + startY, tmpStr.c_str());
                }
            }
            tmpStr.clear();
        }
    }
}



/**
 * 判断生活指数是否适宜
 * https://seniverse.yuque.com/hyper_data/api_v3/qa0mpo
 * @param 生活指数值
 * @return "适宜"类返回 64（代表√），"不适宜"类返回68（代表×），否则返回 67（代表☑️，暂缺的意思）
 */
int parseIconicCheck(String str)
{
    if (str == F("适宜") ||      //
        str == F("较适宜") ||    //
        str == F("一般"))
    {
        return 64;               //"√";
    }
    if (str == F("较不宜") ||    //
        str == F("不太适宜") ||  //
        str == F("不适宜"))
    {
        return 68;               //"×";
    }
    return 67;                   //"☑️";
}

int calStartAt(int areaMaxLen, int contentLen)
{
    return areaMaxLen > contentLen ? (areaMaxLen - contentLen) / 2 : 0;
}
