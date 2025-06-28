# My take on making a Macropad

This project consists of a pair of a device and a companion application.
The `Macropad Companion` application is used to extend the macropad's capabilities by defining custom actions. (WIP)

### Firmware
The device is running [QMK Firmware](https://qmk.fm/), for more info refer to [Firmware/readme](Firmware/readme.md).

### Companion Application

`Macropad Companion` is a QtQuick application that opens the device as a raw HID device (using [hidapi](https://github.com/libusb/hidapi)) and listens to events from it.
