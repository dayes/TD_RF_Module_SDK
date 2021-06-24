LAN TX example.

This example project use EFM32 CMSIS, the emlib peripheral library,
the libttdcore utility library and the libtfrf library to demonstrate
a simple SIGFOX transmitter that sends both a binary and string message to the
SIGFOX network upon startup.

On the TDxxxx EVB, the blue LED connected to the TDxxxx RF module TIM2
pin is also turned on during the SIGFOX message transmission.
