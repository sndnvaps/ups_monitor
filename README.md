# ups_monitor for raspberry pi 

## before build it ,need to install aarch64-none-linux-gnu to system

## how to build it -> cross-compile with windows

```bash
cargo build --release --target=aarch64-unknown-linux-musl
```

## upload to the pi with pscp command

```
F:\rust\ups_monitor_rust>pscp -scp F:\rust\ups_monitor_rust\target\
aarch-unknown-linux-musl\release\ups_monitor_rust root@192.168.xx.110:/usr/bin/
```

## test it on the pi

```bash
root@Rpi:/usr/bin# ups_monitor_rust
```

output just look like

```json
Debug: PowerMonitor 
{
        Voltage = 8.204V,
        Shunt_Voltage = 528µV,
        Current = 1.730A,
        Power = 711mW 
}
Debug: PowerMonitor 
{
        Voltage = 8.204V,
        Shunt_Voltage = 525µV,
        Current = 1.720A,
        Power = 706mW 
}
Debug: PowerMonitor 
{
        Voltage = 8.204V,
        Shunt_Voltage = 545µV,
        Current = 1.786A,
        Power = 706mW 
}
```
