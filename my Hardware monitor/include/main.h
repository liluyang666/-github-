// MAIN.h
#ifndef MAIN_H
#define MAIN_H

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <string>
#include <SSD1306Wire.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WebServer.h>
#include <Ticker.h>
#include <EEPROM.h>
#include <cstdlib>
#include <iomanip>
#include <stdlib.h>
#include "main.h"
#include "fonts.h"
#include "e2prom.h"

// 是否清空E2PROM
#define ERASE_ALL_EEPROM 0

// 状态
#define CONNECT_WIFI 0
#define HTTP_CLIENT_REQUEST 1
#define SERVER_HANDLE_CLIENT 2

// 地址定义
#define SSID_ADDR 0
#define PASSWORD_ADDR 50
#define URL_ADDR 100
#define REFRESH_TIME_ADDR 150

// 蜂鸣器
#define BUZZER_PIN D4
#define LED_PIN D4

// OLED
#define I2C_ADDR 0x3c // oled屏幕的I2c地址
#define SDA_PIN 4     // SDA引脚，默认gpio4(D2)
#define SCL_PIN 5     // SCL引脚，默认gpio5(D1)

// AP模式下热点的账号和密码
extern const char *ap_ssid;
extern const char *ap_password;

// 声明
extern ESP8266WiFiMulti wifiMulti;
extern HTTPClient httpClient;
extern Ticker timer;
extern String URL;
extern ESP8266WebServer server;
extern float refresh_time;
extern unsigned char STATE;
extern int wifiDisconnectedTime;
extern unsigned char timer_flag;
extern SSD1306Wire oled;

// 声明
void wifiConnectedAnimation(void);
void timerCallback();
void informationDisplay(void);
void errorDisplay(int httpCode);
void disconnectDisplay(void);
void networkUnableAccessDisplay(void);
void pleaseConnectAPDisplay();
void wifiConnectingAnimation(void);
void startAnimation(void);
void connectWiFi();
void httpClientRequest();
void handle_root();
void handle_wifi();
String readStringFromEEPROM(int address);
void eraseStringInEEPROM(int address);
void cpuDisplay(const char *cpu_temp, const char *cpu_usage_percent, const char *memory_usage_percent, const char *disk_usage_percent);
void cpuTempDisplay(const char *cpu_temp);
void cpuUsagePercentDisplay(const char *cpu_usage_percent);
void memoryUsagePercentDisplay(const char *memory_usage_percent);
void diskUsagePercentDisplay(const char *disk_usage_percent);
void gpuDisplay(JsonDocument &doc, int gpuIndex);
void gpuIdDisplay(JsonDocument &doc, int gpuIndex);
void gpuTemperatureDisplay(JsonDocument &doc, int gpuIndex);
void writeStringToEEPROM(int address, const String &str);

#endif // MAIN_H