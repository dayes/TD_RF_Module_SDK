SIGFOX downlink example.

This example project use EFM32 CMSIS, the emlib peripheral library, the
libttdcore utility library and the libtfrf library to demonstrate a device that
sends a message to a the SIGFOX network upon startup and wait for a downlink
response.

On the TDxxxx EVB, the blue LED connected to the TDxxxx RF module TIM2 pin is
also turned on during the SIGFOX transmission.

Please note that the device must be attached to a SIGFOX contract that includes
bidirectional transmissions, and that the down-link contents must be properly
configured on the SIGFOX backend and eventually on the application server too.
