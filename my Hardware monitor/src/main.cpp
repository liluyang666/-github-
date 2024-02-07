#include "main.h"
// 变量初始化
ESP8266WebServer server(80);
ESP8266WiFiMulti wifiMulti;
HTTPClient httpClient;
Ticker timer;
String URL;
SSD1306Wire oled(I2C_ADDR, SDA_PIN, SCL_PIN);

unsigned char STATE = CONNECT_WIFI;
float refresh_time = 0;
int wifiDisconnectedTime = 0;
unsigned char timer_flag = 0;
const char *ap_ssid = "ESP8266_AP";
const char *ap_password = "password";

void setup()
{
    tone(BUZZER_PIN, 1000);
    noTone(BUZZER_PIN);
    Serial.begin(115200);

    WiFi.softAP(ap_ssid, ap_password);
    IPAddress local_IP(192, 168, 4, 1);
    IPAddress gateway(192, 168, 4, 1);
    IPAddress subnet(255, 255, 255, 0);
    WiFi.softAPConfig(local_IP, gateway, subnet);
    server.on("/", handle_root);
    server.on("/wifi", handle_wifi);
    server.begin();
    EEPROM.begin(512);
    if (readStringFromEEPROM(SSID_ADDR) == "\0")
    {
        STATE = HTTP_CLIENT_REQUEST;
    }
    if (ERASE_ALL_EEPROM)
    {
        for (int i = 0; i < 513; i++)
        {                       // 遍历EEPROM的每个地址
            EEPROM.write(i, 0); // 将每个地址的内容设置为0
        }
        EEPROM.commit();
    }
    URL = readStringFromEEPROM(URL_ADDR);
    oled.init();
    oled.flipScreenVertically(); // 设置屏幕翻转
    oled.setContrast(255);       // 设置屏幕亮度
    startAnimation();            // 开机动画
    oled.clear();
    oled.display();                 // 清除屏幕
    oled.setFont(ArialMT_Plain_16); // 设置字体
    // connectWiFi();                   //连接WiFi
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH);
    timer.attach_ms(1000, timerCallback);
    pinMode(D3, INPUT);
    digitalWrite(D3, HIGH);
}

void loop()
{
    if (digitalRead(D3) == LOW)
    {
        WiFi.mode(WIFI_AP);
    }
    switch (STATE)
    {
    case CONNECT_WIFI:
        connectWiFi();
    case HTTP_CLIENT_REQUEST:
        httpClientRequest(); // 通过客户端发送http请求，并响应
    }
    server.handleClient();
}

/*http连接*/
void httpClientRequest()
{
    WiFiClient client; // 新添加
    httpClient.begin(client, URL);
    int httpCode = httpClient.GET(); // 通过GET函数启动连接并发送HTTP请求
    if (httpCode > 0)
    {
        if (WiFi.status() == WL_CONNECTED)
        {
            delay(1000);
            if (client.connect("www.baidu.com", 443))
            {
                Serial.print("URL: ");
                Serial.println(URL);
                httpClient.begin(client, URL);
                // httpCode = httpClient.GET(); // 通过GET函数启动连接并发送HTTP请求
                if (httpCode > 0)
                {
                    timer_flag = 0;
                    Serial.printf("[HTTP] GET... code: %d\n", httpCode);
                    if (httpCode == HTTP_CODE_OK)
                    {
                        digitalWrite(LED_PIN, LOW);
                        delay(100);
                        digitalWrite(LED_PIN, HIGH);
                        informationDisplay();
                    }
                    else
                    {
                        errorDisplay(httpCode);
                    }
                }
                else
                {
                    Serial.println(httpCode);
                    disconnectDisplay(); // 显示unable to connect……
                    // timer_flag = 1;
                }
                httpClient.end();
            }
            else
            {
                networkUnableAccessDisplay(); // 显示unable to access network
                // timer_flag = 1;
            }
        }
        else
        {
            pleaseConnectAPDisplay();
            // timer_flag = 1;
        }
    }
    else
    {
        pleaseConnectAPDisplay();
        // timer_flag = 1;
        if (digitalRead(D3) == LOW)
        {
            WiFi.mode(WIFI_AP);
        }
    }
}

/*自动连接wifi模式*/
void connectWiFi()
{
    unsigned char wifiConnectTime = 0;
    oled.clear();
    Serial.print("Connecting to ");
    Serial.print(WiFi.SSID());
    WiFi.mode(WIFI_STA);
    WiFi.begin(readStringFromEEPROM(SSID_ADDR), readStringFromEEPROM(PASSWORD_ADDR)); // 启动WIFI
    while (WiFi.status() != WL_CONNECTED)
    { // 判断WiFi的连接状态，如果没有连接成功，等待
        delay(1000);
        wifiConnectingAnimation(); // wifi正在连接动画
        wifiConnectTime++;
        if (wifiConnectTime == 10)
        {
            wifiConnectTime = 0;
            WiFi.disconnect();
            WiFi.mode(WIFI_AP);
            Serial.println("WiFi connection stopped.");
            STATE = HTTP_CLIENT_REQUEST;
            Serial.println("STATE = HTTP_CLIENT_REQUEST");
            break;
        }
    }
    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("");
        Serial.println("WiFi connected");
        Serial.print("IP address:");
        Serial.println(WiFi.localIP()); // 获取本机的IP
        wifiConnectedAnimation();
        STATE = HTTP_CLIENT_REQUEST;
    }
}

/*定时器*/
void timerCallback()
{
    if (timer_flag == 1)
    {
        wifiDisconnectedTime++;
        Serial.println(wifiDisconnectedTime);
        if (wifiDisconnectedTime == 90)
        {
            timer_flag = 0;
            wifiDisconnectedTime = 0;
            STATE = CONNECT_WIFI;
            Serial.println("STATE = CONNECT_WIFI");
        }
    }
}

/*html获取wifi和密码和请求地址*/
void handle_root()
{
    int n = WiFi.scanNetworks();
    String ssid_string = readStringFromEEPROM(SSID_ADDR);
    String refresh_time_string = readStringFromEEPROM(REFRESH_TIME_ADDR);
    URL = readStringFromEEPROM(URL_ADDR);
    String html = "<!DOCTYPE html><html><head><meta name='viewport' content='width=device-width, initial-scale=1.0'><style>body {background-color: #89a5cb;text-align: center;font-family: Arial, sans-serif;margin: 0;padding: 0;}.container {display: flex;flex-direction: column;justify-content: center;align-items: center;height: 100vh;}h2 {color: #2632a0;font-size: 40px;font-family: Arial, sans-serif;margin-top: 0;}label{font-size: 30px;}select,input[type=text],input[type=password],input[type=number] {background-color: #fefae0;border: none;border-radius: 30px;padding: 10px 20px;margin: 10px;height: 50px;width: 300px;box-sizing: border-box;font-size: 18px;}.input-wrapper {position: relative;display: inline-block;}#passwordInput {padding-right: 30px;}.toggle-password {position: absolute;right: 0;top: 0;border: none;background: transparent;cursor: pointer;margin-top: 23px;margin-right: 10px;}input[type=submit] {background-color: #0d0667;border: none;border-radius: 30px;color: #ffffff;padding: 10px 20px;margin: 10px;height: 50px;width: 150px;cursor: pointer;font-size: 20px;}</style></head><body><div class='container'><h2>Connect to WiFi</h2><form method='POST' action='/wifi'><label>SSID:</label><br>";
    html += "<select name='ssid'>";
    for (int i = 0; i < n; ++i)
    {
        html += "<option value='" + WiFi.SSID(i) + "'>" + WiFi.SSID(i) + " " + WiFi.RSSI() + "</option>";
    }
    html += "</select><br><label>Password:</label><br><div class='input-wrapper'><input type='password' name='password' id='password' placeholder='Enter password'><button type='button' onclick='togglePasswordVisibility()' class='toggle-password'>🙈</button></div><br>";
    html += "<label>URL:</label><br><input type='text' name='url' id='ipAddressInput' placeholder='Enter URL'><br>";
    html += "<label>RefreshTime:</label><br><input type='number' min='5' max='20' name='RefreshTime' value='" + refresh_time_string + "' placeholder='Enter refresh time'><br>";

    html += "<input type='submit' value='Connect'></form></div></body></html>";
    html += "<script>function togglePasswordVisibility() {var passwordInput = document.getElementById('password');var button = document.querySelector('.toggle-password');if (passwordInput.type === 'password') {passwordInput.type = 'text';button.innerHTML = '👁️';} else {passwordInput.type = 'password';button.innerHTML = '🙈';}}</script>";
    html += "<script>document.getElementById('ipAddressInput').value = '" + URL + "';</script>";
    html += "<script>document.addEventListener('DOMContentLoaded', function() {var selectElement = document.querySelector('select');selectElement.querySelector('option[value='" + ssid_string + "']').setAttribute('selected', 'selected');});</script>";

    server.send(200, "text/html", html);
}

/*用获取到的wifi账号密码连接网络*/
void handle_wifi()
{
    String ssid = server.arg("ssid");
    String password;
    if (server.arg("password") != "\0")
    {
        password = server.arg("password");
    }
    else
    {
        password = readStringFromEEPROM(PASSWORD_ADDR);
    }
    URL = server.arg("url");
    eraseStringInEEPROM(SSID_ADDR);
    writeStringToEEPROM(SSID_ADDR, ssid);
    eraseStringInEEPROM(PASSWORD_ADDR);
    writeStringToEEPROM(PASSWORD_ADDR, password);
    eraseStringInEEPROM(URL_ADDR);      // 清除eeprom里的URL
    writeStringToEEPROM(URL_ADDR, URL); // 把刚得到的URL写入到eeprom
    // eraseStringInEEPROM(REFRESH_TIME_ADDR);
    // writeStringToEEPROM(REFRESH_TIME_ADDR, server.arg("RefreshTime"));
    // refresh_time = readStringFromEEPROM(REFRESH_TIME_ADDR).toFloat();
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), password.c_str());
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        wifiConnectingAnimation();
    }
    server.send(200, "text/plain", "Connected to WiFi");
    wifiConnectedAnimation();
    STATE = HTTP_CLIENT_REQUEST;
}