# sonoff-webthing

This project aims at integrating [iTead](https://www.itead.cc/) Sonoff devices
with [Mozilla's proposed Web of Things API](https://iot.mozilla.org/wot/).

It is **bleeding edge** technology and it (currently) only supports:

- [Sonoff Basic (R2)](https://www.itead.cc/smart-home/sonoff-wifi-wireless-switch.html)

## Features

- WiFi configuration _via_ SmartConfig, tested with
  [this Android app](https://play.google.com/store/apps/details?id=com.cmmakerclub.iot.esptouch&hl=en)
- 100% [Web Thing](https://iot.mozilla.org/things/) compliant
- Support [mDNS](https://en.wikipedia.org/wiki/Multicast_DNS)
- Automatic signed firmware update
  ([OTA](https://en.wikipedia.org/wiki/Over-the-air_programming)) by
  short-pressing the main button 10 times
- Main button can be used to turn on/off the switch
- Hard-reset by holding the button for more than 5 seconds
- Visual feedback using the built-in LED

## Usage

This project is a firmware, which require some hardware. You need a iTead sonoff
compatible device. See the list of compatible devices at the top of this file.
In this section, you will find instructions to install (flash) and use the
`sonoff-webthing` firmware (this project).

### Installation

You need [`esptool.py`](https://github.com/espressif/esptool), a FTDI and a USB
cable. Flashing a device might be scary but it is usually straightforward and
relatively simple, especially if you stay calm and focused.

:warning: DO NOT connect your device to mains AC (a.k.a. power coming from your
outlet in your wall) when you want to flash it. This is not needed and EXTREMELY
DANGEROUS.

It is possible that your device requires some hardware preparation. See this
wonderful
[wiki page](https://github.com/arendst/Sonoff-Tasmota/wiki/Hardware-Preparation)
to prepare your device. For example, the Sonoff Basic R2 can be flashed
[by soldering a 4-pin header](https://twitter.com/couac/status/1106286305372184576).

0. Connect your FTDI to your device and your FTDI to your computer using a USB
   cable
1. Backup the current firmware on your device (only needed once):

   ```
   $ esptool.py --port /dev/usb read_flash 0x00000 0x100000 sonoff-orig.bin
   ```

1. Download the latest `sonoff-webthing` firmware located in the
   [`docs/` folder](./docs/)
1. Flash your device:

   ```
   $ esptool.py -p /dev/usb erase_flash
   $ esptool.py -p /dev/usb write_flash -fm dout -fs 1MB 0x00000 sonoff-webthing-signed.bin
   ```

That's it! You can now unplug the FTDI from your device. Replace the PCB in its
enclosure and install the device as indicated in the iTead manual. Ready to
power your device on? Keep reading!

### Configuration

When the device is powered on, the LED should blink slowly: the device is ready
to be connected to your WiFi. Use a _SmartConfig_ application (like
[this one for Android](https://play.google.com/store/apps/details?id=com.cmmakerclub.iot.esptouch&hl=en))
to configure the device with your WiFi credentials.

Once the LED is powered off, your device is fully operational. Some devices like
the Sonoff Basic (R2) can be used manually but not all of them. The best way to
make sure your device is working will be to control it _via_ the Mozilla Things
Gateway.

### Manual usage

- Sonoff Basic (R2): the button can be used as a regular on/off switch.

### Mozilla Things Gateway

The device should automatically appear in the list of Thing devices. If not, try
to reboot it or you can perform a hard reset by pressing the main button for
more than 5 seconds and then releasing it.

### Automatic update (OTA)

Press the main button **10 times** to start the automatic update process. The
LED should blink 5 times. If not, then retry. After that, the device will
download the latest firmware hosted on GitHub. The LED might blink or might be
on for some time, until the device restarts itself.

## Development

:warning: This section is incomplete, sorry for that! I use a `Makefile` and the
command line to build firmware, not the Arduino IDE but this project can be
opened, compiled and flashed _via_ the Arduino IDE. Please follow
[this documentation](https://github.com/arendst/Sonoff-Tasmota/wiki/Arduino-IDE#configure-arduino-ide)
to configure the Arduino IDE.

:warning: This project requires the latest `master` version of
[`webthing-arduino`](https://github.com/mozilla-iot/webthing-arduino) to avoid [this
issue](https://github.com/mozilla-iot/webthing-arduino/issues/59).

Run `make debug` to build a development firmware and `make release` to build a
production firmware.

## License

sonoff-webthing is released under the "Mozilla Public License Version 2.0".
Please see the bundled [`LICENSE` file](./LICENSE).
