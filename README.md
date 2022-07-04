# ups monitor
 ups status monitor for Raspberry Pi Openwrt
 
# connect 
    Raspberry Pi I2C interface

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
	https://www.waveshare.net/wiki/UPS_HAT

# python support from waveshare
	https://www.waveshare.net/w/upload/d/d9/UPS_HAT.7z
