# 该文件包含已经测试过的可以使用的C++标准库及函数

## 1.std::cout
std::cout会默认输出到串口(Serial),未配置时默认波特率为115200
```cpp
#include <iostream>
std::cout << "hello world" << std::endl;
```

c函数参考：https://docs.espressif.com/projects/esp-idf/zh_CN/latest/esp32/api-guides/stdio.html

## 2.stl库容器
std::vector
std::list
std::map
std::queue等都可以使用

## 3.异常处理
```cpp
#include <Arduino.h>
#include <iostream>
void test(int pra){
  if(abs(pra>100)){
    String info = "value:"+String(pra);
    throw std::runtime_error(info.c_str());
  }
}
void setup() {}
void loop() {
  try{
    test(1000);
  } catch (std::exception &e) {
    std::cout << "Error: " << e.what() << std::endl;
  }
  delay(1000);
}
```

