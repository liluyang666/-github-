#include "main.h"
#include "display.h"
#include "fonts.h"

/*开机动画*/
void startAnimation(void)
{
    for (int16_t i = 0; i < oled.getHeight() / 2; i += 2)
    {
        oled.drawRect(i, i, oled.getWidth() - 2 * i, oled.getHeight() - 2 * i);
        oled.display();
        delay(50);
    }
}

/*wifi正在连接动画*/
void wifiConnectingAnimation(void)
{
    oled.clear();
    oled.drawFastImage(0, 0, 128, 1, fangkuang[0]);
    oled.drawFastImage(0, 63, 128, 1, fangkuang[0]);
    oled.drawFastImage(0, 0, 1, 64, fangkuang[1]);
    oled.drawFastImage(127, 0, 1, 64, fangkuang[1]);
    oled.drawString(16, 0, "Connecting to ");
    oled.drawString(4, 16, WiFi.SSID());
    oled.display();
    oled.drawFastImage(48, 32, 32, 32, wifi_connect[2]);
    delay(100);
    oled.display();
    oled.drawFastImage(48, 32, 32, 32, wifi_connect[1]);
    delay(100);
    oled.display();
    oled.drawFastImage(48, 32, 32, 32, wifi_connect[0]);
    delay(100);
    oled.display();
}

/*wifi连接成功动画*/
void wifiConnectedAnimation(void)
{
    oled.clear();
    oled.display();
    oled.drawString(0, 0, "WiFi connected");
    oled.drawString(0, 16, "IP address:");
    String ip_addr = WiFi.localIP().toString().c_str();
    oled.drawString(0, 32, ip_addr);
    oled.display();
    oled.drawFastImage(14, 50, 100, 12, jindutiao[0]);
    oled.display();
    int jindutiao_jiazai = 15;
    int i;
    for (i = 0; i < 98; i++)
    {
        oled.drawFastImage(jindutiao_jiazai, 50, 100, 12, jindutiao[1]);
        oled.display();
        jindutiao_jiazai++;
        delay(10);
    }
}

/*请连接热点并且登录网页显示*/
void pleaseConnectAPDisplay()
{
    oled.clear();
    oled.drawString(0, 0, "Please connect");
    oled.drawString(0, 16, ap_ssid);
    oled.drawString(0, 32, "Please enter");
    String ipStr = WiFi.softAPIP().toString();
    oled.drawString(0, 48, ipStr);
    oled.display();
}
/*服务器信息显示*/
void informationDisplay(void)
{
    JsonDocument doc1, doc2;
    deserializeJson(doc1, httpClient.getString());
    deserializeJson(doc2, httpClient.getString());
    const char *cpu_temp = doc2["Children"]["Children"]["Children"]["Children"]["Value"];
    // const char *cpu_usage_percent = doc2["cpu_usage_percent"];
    // const char *disk_usage_percent = doc2["disk_usage_percent"];
    // const char *memory_usage_percent = doc2["memory_usage_percent"];
    oled.clear();
    cpuTempDisplay(cpu_temp);
    delay(1000);
    oled.display();
    // const char *cpu_temp = doc["cpu_temp"];
    // const char *cpu_usage_percent = doc["cpu_usage_percent"];
    // const char *disk_usage_percent = doc["disk_usage_percent"];
    // const char *memory_usage_percent = doc["memory_usage_percent"];
    // for (i = 0; i <= 9; i++)
    // {
    //     oled.clear();
    //     oled.drawFastImage(87, 0, 1, 64, fangkuang[1]);
    //     oled.drawFastImage(87, 32, 40, 1, fangkuang[0]);
    //     cpuDisplay(cpu_temp, cpu_usage_percent, disk_usage_percent, memory_usage_percent);
    //     gpuDisplay(doc, i);
    //     oled.display();
    //     delay(refresh_time * 1000 / 10);
    // }
}
/*错误代码显示*/
void errorDisplay(int httpCode)
{
    oled.clear();
    Serial.printf("[HTTP] GET... failed,error: %s\n", httpClient.errorToString(httpCode).c_str());
    std::string httpCodeString = std::to_string(httpCode);
    JsonDocument doc;
    deserializeJson(doc, httpClient.getString());
    String error = doc["error"];
    oled.drawString(0, 0, &httpCodeString[0]);
    oled.drawString(0, 16, error);
    oled.display();
}
/*未连接到服务器显示*/
void disconnectDisplay(void)
{
    oled.clear();
    Serial.printf("[HTTP] Unable to connect\n");
    oled.drawString(0, 0, "[HTTP] Unable to");
    oled.drawString(0, 16, " connect......");
    oled.display();
    digitalWrite(LED_PIN, LOW);
    delay(70);
    digitalWrite(LED_PIN, HIGH);
    delay(70);
    digitalWrite(LED_PIN, LOW);
    delay(70);
    digitalWrite(LED_PIN, HIGH);
    delay(70);
    digitalWrite(LED_PIN, LOW);
    delay(70);
    digitalWrite(LED_PIN, HIGH);
    delay(70);
}
/*未连接到广域网显示*/
void networkUnableAccessDisplay(void)
{
    oled.clear();
    oled.drawString(0, 0, "Wifi connected");
    oled.drawString(0, 16, "Unable to");
    oled.drawString(0, 32, "access network");
    oled.display();
}
/*CPU信息显示*/
void cpuDisplay(const char *cpu_temp, const char *cpu_usage_percent, const char *memory_usage_percent, const char *disk_usage_percent)
{
    /*温度*/
    cpuTempDisplay(cpu_temp);
    /*CPU*/
    cpuUsagePercentDisplay(cpu_usage_percent);
    /*内存*/
    memoryUsagePercentDisplay(memory_usage_percent);
    /*硬盘*/
    diskUsagePercentDisplay(disk_usage_percent);
}
void cpuTempDisplay(const char *cpu_temp)
{
    oled.drawFastImage(0, 0, 16, 16, text[0]);
    oled.drawFastImage(16, 0, 16, 16, text[1]);
    oled.drawString(32, 0, ":");
    if (strlen(&cpu_temp[0]) == 3)
    {
        oled.drawString(44, 0, cpu_temp);
        oled.drawFastImage(70, 0, 16, 16, text[20]);
    }
    else if (strlen(&cpu_temp[0]) == 4)
    {
        oled.drawString(36, 0, cpu_temp);
        oled.drawFastImage(70, 0, 16, 16, text[20]);
    }
    else if (strlen(&cpu_temp[0]) == 5)
    {
        oled.drawFastImage(36, 0, 16, 16, text[21]);
        oled.drawFastImage(36, 0, 16, 16, text[22]);
    }
}
void cpuUsagePercentDisplay(const char *cpu_usage_percent)
{
    oled.drawFastImage(0, 16, 16, 16, text[2]);
    oled.drawFastImage(10, 16, 16, 16, text[3]);
    oled.drawFastImage(20, 16, 16, 16, text[4]);
    oled.drawString(32, 16, ":");
    if (strlen(&cpu_usage_percent[0]) == 3)
    {
        oled.drawString(44, 16, cpu_usage_percent);
    }
    else if (strlen(&cpu_usage_percent[0]) == 4)
    {
        oled.drawString(36, 16, cpu_usage_percent);
    }
    else if (strlen(&cpu_usage_percent[0]) == 5)
    {
        oled.drawString(36, 16, "99.9");
    }
    oled.drawString(70, 16, "%");
}
void memoryUsagePercentDisplay(const char *memory_usage_percent)
{
    oled.drawFastImage(0, 32, 16, 16, text[18]);
    oled.drawFastImage(16, 32, 16, 16, text[19]);
    oled.drawString(32, 32, ":");
    if (strlen(&memory_usage_percent[0]) == 3)
    {
        oled.drawString(44, 32, memory_usage_percent);
    }
    else if (strlen(&memory_usage_percent[0]) == 4)
    {
        oled.drawString(36, 32, memory_usage_percent);
    }
    else if (strlen(&memory_usage_percent[0]) == 5)
    {
        oled.drawString(36, 32, "99.9");
    }
    oled.drawString(70, 32, "%");
}
void diskUsagePercentDisplay(const char *disk_usage_percent)
{
    oled.drawFastImage(0, 48, 16, 16, text[5]);
    oled.drawFastImage(16, 48, 16, 16, text[6]);
    oled.drawString(32, 48, ":");
    if (strlen(&disk_usage_percent[0]) == 3)
    {
        oled.drawString(44, 48, disk_usage_percent);
    }
    else if (strlen(&disk_usage_percent[0]) == 4)
    {
        oled.drawString(36, 48, disk_usage_percent);
    }
    else if (strlen(&disk_usage_percent[0]) == 5)
    {
        oled.drawString(36, 48, "99.9");
    }
    oled.drawString(70, 48, "%");
}
/*GPU信息显示*/
void gpuDisplay(JsonDocument &doc, int gpuIndex)
{
    /*GPU id*/
    oled.drawFastImage(96, 0, 24, 16, text[23]);
    gpuIdDisplay(doc, gpuIndex);
    /*GPU 温度*/
    oled.drawFastImage(99, 34, 16, 16, text[24]);
    gpuTemperatureDisplay(doc, gpuIndex);
}
void gpuIdDisplay(JsonDocument &doc, int gpuIndex)
{
    JsonObject gpu_info = doc["gpu_info"][gpuIndex];
    oled.drawString(103, 16, gpu_info["id"]);
}
void gpuLoadDisplay(JsonDocument &doc, int gpuIndex)
{
    JsonObject gpu_info = doc["gpu_info"][gpuIndex];
    oled.drawString(0, 0, gpu_info["load"]); // 在oled上的显示位置需要定义
}
void gpuMemoryUsageDisplay(JsonDocument &doc, int gpuIndex)
{
    JsonObject gpu_info = doc["gpu_info"][gpuIndex];
    oled.drawString(0, 0, gpu_info["memory_usage_percent"]); // 在oled上的显示位置需要定义
}
void gpuTemperatureDisplay(JsonDocument &doc, int gpuIndex)
{
    JsonObject gpu_info = doc["gpu_info"][gpuIndex];
    oled.drawString(99, 48, gpu_info["temp"]);
}
