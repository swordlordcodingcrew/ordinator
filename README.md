# ordinator
Extendable firmware for the LILYGO TTGO T-Wristband





## howto
Initialise the project. Already done, won't be needed anymore

> pio project init --ide clion --board esp32dev

To use CLion as an IDE, or when things wont work as expected, run this command:

> pio project init --ide clion
>

To update middleware
> pio update
>

To compile

> pio run
>

To upload to the device (attached via USB)
> pio run -t upload
>

To start the serial monitor
> pio device monitor
>