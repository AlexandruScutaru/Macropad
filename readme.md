# My take on making a Macropad

This project consists of a pair of a device and a companion application.
The `Macropad Companion` application is used to extend the macropad's capabilities by defining custom actions. (WIP)

There is also a small backend/rest-api project used to save and load profiles from the cloud.

### Firmware
The device is running [QMK Firmware](https://qmk.fm/), for more info refer to [Firmware/readme](Firmware/readme.md).

### Companion Application

`Macropad Companion` is a QtQuick application that opens the device as a raw HID device (using [hidapi](https://github.com/libusb/hidapi)) and listens to events from it.

### Backend

This is a simple python flask REST API used to register and log users in so that they can save and load profiles from across different devices.
