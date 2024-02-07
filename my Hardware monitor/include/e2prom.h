// E2PROM.h
#ifndef E2PROM
#define E2PROM

#include <Arduino.h> // 包含Arduino基础头文件

// 向EEPROM中写入字符串数据的函数声明
void writeStringToEEPROM(int address, const String &str);

// 从EEPROM中读取字符串数据的函数声明
String readStringFromEEPROM(int address);

// 从EEPROM中清除字符串数据的函数声明
void eraseStringInEEPROM(int address);

#endif // E2PROM
