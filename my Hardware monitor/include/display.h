#ifndef DISPLAY_H
#define DISPLAY_H

#include <ArduinoJson.h>
#include <SSD1306Wire.h>

// 声明函数
void startAnimation(void);
void wifiConnectingAnimation(void);
void wifiConnectedAnimation(void);
void pleaseConnectAPDisplay();
void informationDisplay();
void errorDisplay(int httpCode);
void disconnectDisplay();
void networkUnableAccessDisplay();
void cpuDisplay(const char *cpu_temp, const char *cpu_usage_percent, const char *memory_usage_percent, const char *disk_usage_percent);
void cpuTempDisplay(const char *cpu_temp);
void cpuUsagePercentDisplay(const char *cpu_usage_percent);
void memoryUsagePercentDisplay(const char *memory_usage_percent);
void diskUsagePercentDisplay(const char *disk_usage_percent);
void gpuDisplay(JsonDocument &doc, int gpuIndex);
void gpuIdDisplay(JsonDocument &doc, int gpuIndex);
void gpuLoadDisplay(JsonDocument &doc, int gpuIndex);
void gpuMemoryUsageDisplay(JsonDocument &doc, int gpuIndex);
void gpuTemperatureDisplay(JsonDocument &doc, int gpuIndex);

#endif // DISPLAY_H
