<!--
 * @LastEditors: qingmeijiupiao
 * @Description: 
 * @Author: qingmeijiupiao
 * @Date: 2024-08-03 00:09:14
-->
# 我们为什么采用Arduino SDK+PlatformIo+CPP开发
官方仓库：https://github.com/espressif/arduino-esp32
## 什么是ArduinoSDK
Arduino SDK是ESP32的开发框架，是用于esp32开发的一系列兼容Arduino IDE API的库
包括:文件目录，标准引脚函数等
* 使用Arduino SDK可以让你不太关注硬件控制的具体实现，只需要关注开发的逻辑即可,
* 当然也会损失一些性能和对底层的控制
我们为什么要采用Arduino SDK呢,因为我们希望大家更注重代码逻辑和算法而不是硬件控制
* Arduino SDK+CPP开发带来的效率的提高和代码可靠性的提升
让我们认为牺牲性能和底层控制来换取更高的开发效率至少不是一件坏事

## 使用Arduino SDK就不能用底层的库了吗
Arduino SDK仅仅是对底层的库的封装，其仍然依赖IDF的标准库,所以我们依然可以去使用标准库
在后面的教程中我们也会大量使用IDF的标准API

## 为什么是PlatformIo
PlatformIo是相比于Arduino IDE更加专业，提供了更多的功能，例如单元测试，git库管理等
更加适合团队开发

## Arduino API 参考
https://www.arduino.cc/reference/en/

## esp-idf API 参考
https://docs.espressif.com/projects/esp-idf/zh_CN/latest/esp32/api-reference/index.html