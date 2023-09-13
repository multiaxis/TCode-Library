# TCode Library
A library which provides methods for decoding TCode and handling the backend.

please find the documention for the classes and functions below
[Link to the documention](https://multiaxis.github.io/TCode-Library/)

---

# Toy code ("T-code")
[Link to the source for this documention on "T-Code"](https://github.com/multiaxis/tcode-spec)

Created by [Tempest For the OSR-2, SR-6, and other DIY toys](https://patreon.com/tempestvr)

---

## Introduction

T-code is a protocol for implementing UART serial communications to an adult toy. It is partly influenced by G-code, which is an alphanumeric format used to drive CNC machines, including 3D-printers.

The toy or “device” will be capable of one or more functions, which are described as Linear motion, Vibration or Rotation “channels”, which can be addressed independently.

Commands are send to the device in the form of alphanumeric ASCII phrases, which are interpreted by the device, stored in a buffer and executed on the receipt of new line or ‘/n’ character.

## 
