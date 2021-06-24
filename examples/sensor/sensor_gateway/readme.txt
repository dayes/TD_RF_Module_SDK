Sensor Gateway template for new TDxxxx RF module projects.

This example project use EFM32 CMSIS, the emlib peripheral library, the
libttdcore utility library and the libtdsensor library monitoring capabilities
to provide a skeleton Sensor Gateway (LAN master and SIGFOX gateway) application
template.

The Sensor Gateway will enter into registration mode for 30 s upon startup,
accepting any ne Device up until the timout occurs or the maximum number of
Devices is reached.