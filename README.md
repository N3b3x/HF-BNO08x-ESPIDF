# HF-BNO08x-ESPIDF

This repository packages the [HF-BNO08x](https://github.com/N3b3x/HF-BNO08x)
library as an ESP‑IDF component targeting the ESP32‑C6.  The component lives in
`components/bno08x` and builds the upstream driver along with a simple I²C
transport implementation.

The upstream project provides a full C++ driver and vendor SH‑2 library for the
BNO08x family of IMUs.  Here it is wrapped for easy use within any ESP‑IDF
application.

See `components/bno08x/README.md` for configuration options and an example of
using the driver in your project.
