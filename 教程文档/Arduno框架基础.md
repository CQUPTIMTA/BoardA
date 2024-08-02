<!--
 * @LastEditors: qingmeijiupiao
 * @Description: 
 * @Author: qingmeijiupiao
 * @Date: 2024-08-03 00:34:08
-->
# PlatformIO Ardunio框架文件基础
一个基本的工程包含
```
.
├── .pio
│   └── （包含PlatformIO的配置文件和项目数据）
├── .vscode
│   └── （包含Visual Studio Code的配置文件）
├── .include
│   └── （用于存放头文件）
├── .lib
│   └── （用于存放库文件）
├── .src
│   └── main.cpp//默认生成的源文件
├── .test
│   └── （可能包含测试文件）
└── platformio.ini
    └── （PlatformIO项目的配置文件）
```
我们主要使用include src文件夹和platformio.ini文件
# main.cpp
必须包含void setup()和void loop()函数

建议包含<Arduino.h>头文件

## void setup()
芯片上电后会调用此函数

## void loop()
当setup()函数执行完毕会循环执行此函数

# platformio.ini
用于配置工程的配置项,例如芯片型号,串口监视器的波特率等

下面是一个基本的ESP32S3开发板配置项
```
[env:esp32-s3-devkitm-1]
platform = espressif32
board = esp32-s3-devkitm-1
framework = arduino
build_flags = -DARDUINO_USB_CDC_ON_BOOT=1   ; 启用usb模拟串口
monitor_speed = 115200 ; 串口监视器波特率
```

# 文件类型支持
## PlatformIO 支持各种c c++文件类型的编译

### 例如.c .cpp .h .hpp .hc
### 注意.h .hpp .hc文件放在include文件夹下
### .c .cpp文件放在src文件夹下

## c/c++ 混合编译
一般不推荐c和c++混合编译，建议全使用c++标准和cpp hpp文件

非要混合编译建议在c模块加上
```
extern "C"{
    //some c code
}
```

# 延时
## Esp32 Arduino库中的delay()函数是被重写的不会阻塞其他任务
函数原型
```
void delay(uint32_t ms)
{
    vTaskDelay(ms / portTICK_PERIOD_MS);
}
```
## 但是delayMicroseconds()是会阻塞其他任务的
函数原型
```
void ARDUINO_ISR_ATTR delayMicroseconds(uint32_t us)
{
    uint64_t m = (uint64_t)esp_timer_get_time();
    if(us){
        uint64_t e = (m + us);
        if(m > e){ //overflow
            while((uint64_t)esp_timer_get_time() > e){
                NOP();
            }
        }
        while((uint64_t)esp_timer_get_time() < e){
            NOP();
        }
    }
}
```


