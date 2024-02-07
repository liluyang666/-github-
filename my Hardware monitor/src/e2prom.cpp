#include <string>
#include <EEPROM.h>
#include "e2prom.h"

/*向EEPROM中写入数据*/
void writeStringToEEPROM(int address, const String &str)
{
    int length = str.length();
    // 写入字符串长度到EEPROM
    EEPROM.write(address, length);
    for (int i = 0; i < length; i++)
    {
        EEPROM.write(address + 1 + i, str[i]); // 然后存储字符串本身
    }
    // 提交更改保存到EEPROM
    EEPROM.commit();
}

/*从EEPROM中读取数据*/
String readStringFromEEPROM(int address)
{
    int len = EEPROM.read(address); // 首先读取字符串长度
    String str = "";
    for (int i = 0; i < len; i++)
    {
        str += char(EEPROM.read(address + 1 + i)); // 然后读取字符串本身
    }
    return str;
    EEPROM.end();
}

/*从EEPROM中清除数据*/
void eraseStringInEEPROM(int address)
{
    int len = EEPROM.read(address);
    for (int i = address; i < len; i++)
    {                       // 遍历EEPROM的每个地址
        EEPROM.write(i, 0); // 将每个地址的内容设置为0
    }
    EEPROM.commit();
}