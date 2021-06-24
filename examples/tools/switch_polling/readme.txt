Switch Monitoring Application Example

This example project use EFM32 CMSIS, the emlib peripheral library, the
libttdcore utility library and the libtdsensor library monitoring capabilities
to provide a skeleton Sensor Transmitter application template.

This example registers the Transmitter to the Sensor backend, then start
monitoring battery, temperature and witch events while sending periodic
keepalive messages every 24 hours.


