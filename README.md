# sonoff-webthing

This project aims at integrating [iTead](https://www.itead.cc/) Sonoff devices
with [Mozilla's proposed Web of Things API](https://iot.mozilla.org/wot/).

It is **bleeding edge** technology and it (currently) only supports:

- [Sonoff Basic (R2)](https://www.itead.cc/smart-home/sonoff-wifi-wireless-switch.html)

## Features

- WiFi configuration _via_ SmartConfig, tested with [this Android
  app](https://play.google.com/store/apps/details?id=com.cmmakerclub.iot.esptouch&hl=en)
- 100% [Web Thing](https://iot.mozilla.org/things/) compliant
- Support [mDNS](https://en.wikipedia.org/wiki/Multicast_DNS)
- Automatic signed firmware update
  ([OTA](https://en.wikipedia.org/wiki/Over-the-air_programming)) by
  short-pressing the main button 10 times
- Main button can be used to turn on/off the switch
- Hard-reset by holding the button for more than 5 seconds
- Visual feedback using the built-in LED

## Usage

:warning: This section is incomplete, sorry for that!

### Installation

1. Download the latest firmware in the [`docs/` folder](./docs/)
2. Backup the current firmware on your device:

    ```
    $ esptool.py --port /dev/usb read_flash 0x00000 0x100000 sonoff-orig.bin
    ```

3. Flash your device:

    ```
    $ esptool.py -p /dev/usb erase_flash
    $ esptool.py -p /dev/usb write_flash -fm dout -fs 1MB 0x00000 sonoff-webthing-signed.bin
    ```

### Setup

Once the device is flashed with the `sonoff-webthing` firmware, replace the PCB
in its enclosure and install the device as indicated in the iTead manual.

When the device is powered on, the LED should blink slowly: the device is ready
to be connected to your WiFi. Use a _SmartConfig_ application (like [this one
for
Android](https://play.google.com/store/apps/details?id=com.cmmakerclub.iot.esptouch&hl=en))
to configure the device with your WiFi credentials.

Once the LED is powered off, your device should be ready.

### Manual usage

The main button can be used as a regular switch.

### Mozilla Things Gateway

The device should automatically appear in the list of Thing devices.

### Automatic update (OTA)

Press the main button **10 times** to start the automatic update process. The
LED should blink 5 times. If not, then retry. After that, the device will
download the latest firmware hosted on GitHub. The LED might blink or might be
on for some time, until the device restarts itself.

## Development

:warning: This section is incomplete, sorry for that! I use a `Makefile` and the
command line to build firmware, not the Arduino IDE but this project can be
opened, compiled and flashed _via_ the Arduino IDE. Please follow [this
documentation](https://github.com/arendst/Sonoff-Tasmota/wiki/Arduino-IDE#configure-arduino-ide)
to configure the Arduino IDE.

:warning: The `webthing-arduino` library requires a change to avoid [this
issue](https://github.com/mozilla-iot/webthing-arduino/issues/59).

Run `make debug` to build a development firmware and `make release` to build a
production firmware.

## License

sonoff-webthing is released under the "Mozilla Public License Version 2.0".
Please see the bundled [`LICENSE` file](./LICENSE).
