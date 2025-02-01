# Flashing ESP-01 using esptool.py

If you don't have an Arduino on hand, it is also possible to flash new firmware to the ESP-01 over any 3.3 V UART bus by using [esptool.py](https://docs.espressif.com/projects/esptool/en/latest/esp8266/index.html) directly (Arduino uses the same tool under the hood).

The following example uses a Raspberry Pi. If you are using a different UART device, skip to the "Enter ESP-01 bootloader" section.

## Configuring Raspberry Pi for use with esptool.py

All Raspberry Pi models have at least one easily accessible UART on the GPIO header. However, by default it is configured to provide a Linux console. In order to use the UART for something else, the terminal must first be disabled. Consult the [Raspberry Pi documentation](https://www.raspberrypi.com/documentation/computers/configuration.html#disabling-the-linux-serial-console) for up-to-date instructions on how to do this.

## Connect Raspberry Pi to ESP-01

With the RPi's Linux console disabled, connect the ESP-01 to the RPi per the following table:

| Raspberry Pi      | ESP-01          |
|-------------------|-----------------|
| GPIO 14 TXD       | RX              |
| GPIO 15 RXD       | TX              |
| 3V3 power         | VCC             |
| Ground            | GND             |
| Ground            | GPIO_0          |
| 3V3 power         | CH_PD or CH_EN  |
| Ground            | RESET           |

## Enter ESP-01 bootloader

To flash new firmware to the ESP-01, it is necessary to first boot to its bootloader. With the ESP-01's GPIO_0 pin pulled low, reset the ESP-01 by briefly pulling its RESET pin high. This causes the ESP-01 to enter bootloader mode.

## Flash firmware with esptool.py

Esptool.py can be installed from PyPI using `pip`:

    pip install esptool

With esptool.py installed, first confirm that you can communicate with the ESP-01. If you are using a Raspberry Pi, use the primary UART, `/dev/serial0`:

    esptool.py --port /dev/serial0 chip_id

You should see something like this:

    esptool.py v4.8.1
    Serial port /dev/serial0
    Connecting...
    Failed to get PID of a device on /dev/ttyS0, using standard reset sequence.

    Detecting chip type... Unsupported detection protocol, switching and trying again...
    Connecting...
    Failed to get PID of a device on /dev/ttyS0, using standard reset sequence.

    Detecting chip type... ESP8266
    Chip is ESP8266EX
    Features: WiFi
    Crystal is 26MHz
    MAC: e8:68:e7:88:59:35
    Stub is already running. No upload is necessary.
    Chip ID: 0x00885935
    Hard resetting via RTS pin...

If you see an error, ensure the ESP-01 is in bootloader mode.

Now it's time to flash the new firmware. First build or download the firmware image, `ESP01Firmware.bin`. Then write it to the ESP-01:

    esptool.py --port /dev/serial0 write_flash 0x0000 ESP01Firmware.bin

You should see something like this:

    esptool.py v4.8.1
    Serial port /dev/serial0
    Connecting...
    Failed to get PID of a device on /dev/ttyS0, using standard reset sequence.

    Detecting chip type... Unsupported detection protocol, switching and trying again...
    Connecting...
    Failed to get PID of a device on /dev/ttyS0, using standard reset sequence.

    Detecting chip type... ESP8266
    Chip is ESP8266EX
    Features: WiFi
    Crystal is 26MHz
    MAC: e8:68:e7:88:59:35
    Stub is already running. No upload is necessary.
    Configuring flash size...
    Flash will be erased from 0x00000000 to 0x00044fff...
    Compressed 279936 bytes to 206898...
    Wrote 279936 bytes (206898 compressed) at 0x00000000 in 18.6 seconds (effective 120.2 kbit/s)...
    Hash of data verified.

    Leaving...
    Hard resetting via RTS pin...

Now pull the ESP-01's GPIO_0 pin high and reset it. If the new firmware was flashed successfully, it will start broadcasting an SSID called "PSLab".
