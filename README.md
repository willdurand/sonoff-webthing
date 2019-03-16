# sonoff-webthing

This project aims at integrating [iTead](https://www.itead.cc/) Sonoff devices
with [Mozilla's proposed Web of Things API](https://iot.mozilla.org/wot/).

It is **bleeding edge** technology and it (currently) only supports:

- [Sonoff Basic (R2)](https://www.itead.cc/smart-home/sonoff-wifi-wireless-switch.html)

## Features

- WiFi configuration _via_ SmartConfig, tested with [this Android
  app](https://play.google.com/store/apps/details?id=com.cmmakerclub.iot.esptouch&hl=en)
- [mDNS](https://en.wikipedia.org/wiki/Multicast_DNS)-ready
- Main button can be used to turn on/off the switch
- 100% [Web Thing](https://iot.mozilla.org/things/) compliant
- Automatic signed firmware update
  ([OTA](https://en.wikipedia.org/wiki/Over-the-air_programming)) by
  short-pressing the main button 10 times
- Hard-reset by holding the button for more than 5 seconds
- Visual feedback using the built-in LED

## Usage

Flash procedure:

```
$ esptool.py -p /dev/usb erase_flash
$ esptool.py -p /dev/usb write_flash -fm dout -fs 1MB 0x00000 build/sonoff-webthing-signed.bin
```

## Development


## License

sonoff-webthing is released under the "Mozilla Public License Version 2.0".
Please see the bundled [`LICENSE` file](./LICENSE).
