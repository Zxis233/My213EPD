#include "Config.h"

extern String authcode_default;
extern String ssid;
extern String pass;
extern int modeNum;
extern int keepNum;
boolean offline_mode = false;


void SmartConfig();  // 微信配网
void wifi_init();    // wifi初始化

void wifi_configure()
{
    if (offline_mode)
    {
        return;
    }
    digitalWrite(D5, LOW);
    wifi_init();

    Serial.println("WiFi Init End!");
    Serial.printf("wifiUnconfigure: %d\r\n", wifiUnconfigure);
    if (wifiUnconfigure)
    {
        int modeButton = HIGH;
        int keepButton = HIGH;

        boolean keepState = true;
        display.setFullWindow();
        display.fillScreen(GxEPD_WHITE);
        u8g2Fonts.setFont(u8g2_font_wqy14_t_gb2312a);
        u8g2Fonts.setForegroundColor(GxEPD_BLACK);  // 设置前景色
        u8g2Fonts.setBackgroundColor(GxEPD_WHITE);  // 设置背景色

        display.setRotation(2);
        display.setBorder(0x77);
        display.fillScreen(GxEPD_WHITE);
        display.drawInvertedBitmap(0, 0, smartconfig, 122, 122, GxEPD_BLACK);

        u8g2Fonts.setBackgroundColor(GxEPD_WHITE);  // 设置背景色
        // u8g2Fonts.setForegroundColor(GxEPD_RED);  // 设置前景色
        u8g2Fonts.setForegroundColor(GxEPD_BLACK);

        u8g2Fonts.setCursor(5, 140);
        u8g2Fonts.print("请扫描二维码配网");
        u8g2Fonts.setCursor(5, 158);
        u8g2Fonts.print("协议为SmartConfig");
        u8g2Fonts.setCursor(5, 182);
        u8g2Fonts.setForegroundColor(GxEPD_RED);
        u8g2Fonts.print("仅可连接2.4GHz的");
        u8g2Fonts.setCursor(5, 200);
        u8g2Fonts.print("WiFi网络");

        u8g2Fonts.setCursor(5, 222);
        u8g2Fonts.setForegroundColor(GxEPD_BLACK);
        u8g2Fonts.print("按");
        u8g2Fonts.setForegroundColor(GxEPD_RED);
        u8g2Fonts.print("Mode键");
        u8g2Fonts.setForegroundColor(GxEPD_BLACK);
        u8g2Fonts.print("离线使用");

        u8g2Fonts.setCursor(6, 242);
        u8g2Fonts.setForegroundColor(GxEPD_BLACK);
        u8g2Fonts.print("按");
        u8g2Fonts.setForegroundColor(GxEPD_RED);
        u8g2Fonts.print("Keep键");
        u8g2Fonts.setForegroundColor(GxEPD_BLACK);
        u8g2Fonts.print("开始配置");
        display.nextPage();

        while (1)
        {
            if (!keepState)
            {
                digitalWrite(D4, HIGH);
                Serial.println("SmartConfig Start!");
                SmartConfig();
                Serial.println("SmartConfig End!");
                while (1)
                    ;  // 配置完将自动重启
                break;
            }

            modeButton = digitalRead(modePin);  // 按键
            if (modeButton == LOW)
            {
                digitalWrite(D5, HIGH);
                offline_mode = true;
                Serial.println("Offline Mode Start!");
                modeNum = 1;
                keepNum = 4;
                delay(200);
                digitalWrite(D5, LOW);
                return;
            }

            keepButton = digitalRead(keepPin);  // 按键
            if (keepButton == LOW)
            {
                keepState = false;

                delay(200);
            }
            else
            {
                keepState = true;
            }
        }
    }
}

extern String ssid;
extern String pass;
extern String authcode;

boolean restoreConfig()
{
    Serial.println("Reading EEPROM...");
    if (EEPROM.read(4000) != 0)
    {
        ssid     = "";
        pass     = "";
        authcode = "";
        for (int i = 4000; i < 4032; ++i)
        {
            char ch = EEPROM.read(i);
            // if (isalpha(ch) || isdigit(ch))
            if (ch != 0)
                ssid += char(EEPROM.read(i));
        }
        Serial.print("WiFi: ");
        Serial.println(ssid);
        for (int i = 4032; i < 4064; ++i)
        {
            char ch = EEPROM.read(i);
            // if (isalpha(ch) || isdigit(ch))
            if (ch != 0)
                pass += char(EEPROM.read(i));
        }
        Serial.print("密码: ");
        Serial.println(pass);
        for (int i = 4064; i < 4096; ++i)
        {
            char ch = EEPROM.read(i);
            // if (isalpha(ch) || isdigit(ch) || ch == '-' || ch == '_')
            if (ch != 0)
                authcode += char(EEPROM.read(i));
        }
        if (authcode == "")
        {
            authcode = authcode_default;
        }
        Serial.print("密钥: ");
        Serial.println(authcode);
        WiFi.begin(ssid.c_str(), pass.c_str());
        return true;
    }
    else
    {
        Serial.println("Config not found.");
        return false;
    }
}

boolean checkConnection()
{
    int ckcount = 0;
    Serial.println("正在等待连接");

    while (ckcount < 50)
    {
        if (WiFi.status() == WL_CONNECTED)
        {
            Serial.println("成功连接！");
            return (true);
        }
        delay(100);
        if (ckcount % 5 == 0)
            Serial.print(".");
        ckcount++;
    }
    Serial.println("连接超时.");
    return false;
}


void SmartConfig()  // 微信配网是通过SmartConfig方法来实现的
{
    EEPROM.begin(4096);
    for (int i = 0; i < 4096; i++)
    {
        EEPROM.write(i, 0);
        // Serial.printf("EEPROM Clean: %d\r\n", i);
    }
    EEPROM.commit();
    Serial.println("EEPROM Cleaned!");
    WiFi.mode(WIFI_AP_STA);
    WiFi.beginSmartConfig();                       // 开始SmartConfig，等待手机端发出用户名和密码
    Serial.println("Waiting for Smartconfig...");  // 打印log信息

    while (!WiFi.smartConfigDone())
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("SmartConfig received.");

    // Wait for WiFi to connect to AP
    Serial.println("Waiting for WiFi");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("WiFi Connected.");
    ssid = WiFi.SSID().c_str();
    pass = WiFi.psk().c_str();
    Serial.printf("SSID:%s\r\n", ssid);
    Serial.printf("PSW:%s\r\n", pass);
    for (int i = 4000; i < 4064; ++i)
    {
        EEPROM.begin(4096);
        EEPROM.write(i, 0);
        EEPROM.commit();
    }
    Serial.println("Writing ssid to EEPROM...");
    for (int i = 0; i < ssid.length(); ++i)
    {
        EEPROM.begin(4096);
        EEPROM.write(4000 + i, ssid[i]);
        EEPROM.commit();
    }
    Serial.println("Writing Password to EEPROM...");
    for (int i = 0; i < pass.length(); ++i)
    {
        EEPROM.begin(4096);
        EEPROM.write(4032 + i, pass[i]);
        EEPROM.commit();
    }
    if (authcode_default.length())
    {
        Serial.println("Writing authcode to EEPROM...");
        for (int i = 0; i < authcode_default.length(); ++i)
        {
            EEPROM.begin(4096);
            EEPROM.write(4064 + i, authcode_default[i]);
            EEPROM.commit();
        }
    }
    Serial.printf("AuthCode:%s \r\n", authcode_default);
    EEPROM.end();
    delay(1000);
    ESP.restart();
}

void wifi_init()
{
    display.fillScreen(GxEPD_WHITE);

    u8g2Fonts.setFont(u8g2_font_wqy16_t_gb2312a);
    u8g2Fonts.setCursor(28, 72);
    u8g2Fonts.setForegroundColor(GxEPD_RED);    // 设置前景色
    u8g2Fonts.setBackgroundColor(GxEPD_WHITE);  // 设置背景色
    u8g2Fonts.print("启动中...");

    u8g2Fonts.setFont(u8g2_font_wqy14_t_gb2312a);
    display.drawInvertedBitmap(0, 125, kokona_122, 122, 122, GxEPD_RED);
    // TODO
    // display.nextPage();
    delay(1000);
    if (restoreConfig())
    {
        if (checkConnection())
        {
            wifiUnconfigure = false;
            return;
        }
    }

    EEPROM.begin(4096);
    if (EEPROM.read(4064) == 'S')
    {
        return;
    }
    wifiUnconfigure = true;
}
