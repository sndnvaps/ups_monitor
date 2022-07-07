# ups monitor
 ups status monitor for Raspberry Pi Openwrt
 
# 图片

![UPS-HAT](https://www.waveshare.net/photo/accBoard/UPS-HAT/UPS-HAT-1.jpg)

# connect (openwrt raspbeery pi需要先安装i2c内核驱动的支持)
    Raspberry Pi I2C interface

# hack （如果获取ups连接到树莓派后，用的是哪个地址）
```bash
opkg install  i2c-tools

i2cdetect -y 1
```
![ups-hat-i2c-address](https://www.waveshare.net/w/upload/f/f1/UPS_HAT_I2C.png)

默认设置的地址为 0x42(需要修改 [src/ina219.cpp#L254](src/ina219.cpp#L254))
```cpp
    double persent;
    // init
    INA219 ina219(0x42); //修改0x42为你用自己的ups设置的i2c地址
    while (1)
```

# depend
   before build the ups_monitor project, you need to build libwiringPi first
   https://github.com/sndnvaps/wiringPi-4-openwrt

# how to build
 1. clone this repo to openwrt-sdk folder
    git clone git@github.com:sndnvaps/ups_monitor.git package/utils/ups_monitor
 2. build in the openwrt-sdk top dir
    make package/utils/ups_monitor/compile
 3. ipk output
    $(TOPDIR)/bin/package/aarch64_cortex-a53/base
 4. ipk name ups_monitor_0.1-1_aarch64_cortex-a53.ipk
 
# how to install
    scp ups_monitor_0.1-1_aarch64_cortex-a53.ipk root@openwrt_ip:/tmp/
    opkg install libstdcpp
    opkg install /tmp/ups_monitor_0.1-1_aarch64_cortex-a53.ipk

# wiki
## ups-hat介绍文档
https://www.waveshare.net/wiki/UPS_HAT

## Raspberry Pi OpenWRT打开 I2C支持 
https://www.icode9.com/content-4-1367375.html
    
# python support from waveshare
https://www.waveshare.net/w/upload/d/d9/UPS_HAT.7z


# 声明
 此代码，目前只测试于 <strong>微雪的UPS_HAT</strong>

 编译的时候，需要<strong>C++11</strong>支持

