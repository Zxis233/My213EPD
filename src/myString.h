#ifndef _MYSTR_H_
#define _MYSTR_H_

const char *ldayCN2[] = {
    "今天",  // 0
    "明 ",   // 1
    "后 "    // 2
};


// 中文星期，样式为：周一
const char *wdayCN2[] = {
    "周日",  // 0
    "周一",  // 1
    "周二",  // 2
    "周三",  // 3
    "周四",  // 4
    "周五",  // 5
    "周六"   // 6
};


// 中文星期，样式为：一
const char *wdayCN1[] = {
    "日",  // 0
    "一",  // 1
    "二",  // 2
    "三",  // 3
    "四",  // 4
    "五",  // 5
    "六"   // 6
};


// U8g2天气字体库天气图标对应代码
const int iconicWeather[] = {
    64,  // 0, 阴
    65,  // 1, 多云
    66,  // 2, 晴（晚上）
    67,  // 3, 雨
    68,  // 4, 雪
    69   // 5, 晴（白天）
};


// 心知天气 40种天气代码
// https://seniverse.yuque.com/hyper_data/api_v3/yev2c3
const int weatherCode[] = {
    5,   // 0, 晴（国内城市白天晴）, Sunny
    2,   // 1, 晴（国内城市夜晚晴）, Clear
    5,   // 2, 晴（国外城市白天晴）, Fair
    2,   // 3, 晴（国外城市夜晚晴）, Fair
    1,   // 4, 多云, Cloudy
    1,   // 5， 晴间多云， Partly Cloudy
    1,   // 6, 晴间多云, Partly Cloudy
    1,   // 7, 大部多云, Mostly Cloudy
    1,   // 8, 大部多云, Mostly Cloudy
    0,   // 9, 阴, Overcast
    3,   // 10, 阵雨, Shower
    3,   // 11, 雷阵雨, Thundershower
    3,   // 12, 雷阵雨伴有冰雹, Thundershower with Hail
    3,   // 13, 小雨, Light Rain
    3,   // 14, 中雨, Moderate Rain
    3,   // 15, 大雨, Heavy Rain
    3,   // 16, 暴雨, Storm
    3,   // 17, 大暴雨, Heavy Storm
    3,   // 18, 特大暴雨, Severe Storm
    3,   // 19, 冻雨, Ice Rain
    4,   // 20, 雨夹雪, Sleet
    4,   // 21, 阵雪, Snow Flurry
    4,   // 22, 小雪, Light Snow
    4,   // 23, 中雪, Moderate Snow
    4,   // 24, 大雪, Heavy Snow
    4,   // 25, 暴雪, Snowstorm
    -1,  // 26, 浮尘, Dust
    -1,  // 27, 扬沙, Sand
    -1,  // 28, 沙尘暴, Duststorm
    -1,  // 29, 强沙尘暴, Sandstorm
    -1,  // 30, 雾, Foggy
    -1,  // 31, 霾, Haze
    -1,  // 32, 风, Windy
    -1,  // 33, 大风, Blustery
    -1,  // 34, 飓风, Hurricane
    -1,  // 35, 热带风暴, Tropical Storm
    -1,  // 36, 龙卷风, Tornado
    -1,  // 37, 冷, Cold
    -1,  // 38, 热, Hot
    -1   // 99, 未知, Unknown
};
#endif