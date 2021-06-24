Temperature Monitoring Application Example

This example project use EFM32 CMSIS, the emlib peripheral library, the
libttdcore utility library and the libtdsensor library monitoring capabilities
to implement a temperature monitoring application.

This examples both send to the UART messages when the corresponding thresholds
are reached, and also send to the Sensor backend low/OK temeperature events,
filtering the high temperature ones before sending.

