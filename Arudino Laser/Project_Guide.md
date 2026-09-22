# Arduino Nano Pan/Tilt Laser Controller

A desktop-controlled pan/tilt project using an ELEGOO Arduino-compatible Nano, an Adafruit Mini Pan-Tilt Kit, micro servos, and a KY-008 laser module.

The system is controlled from a Java desktop application over USB serial communication.

> **Safety:** This project uses a laser. Never point the laser at people, animals, vehicles, aircraft, reflective surfaces, or windows. Never look directly into the beam.

---

## Project Architecture

```text
┌──────────────────────┐
│ Java Desktop Control │
│       Panel          │
└──────────┬───────────┘
           │
           │ USB / Serial
           ▼
┌──────────────────────┐
│     ELEGOO Nano      │
│                      │
│ D9  ──► Pan Servo    │
│ D10 ──► Tilt Servo   │
│ D7  ──► Laser        │
└──────────┬───────────┘
           │
           │ Common Ground
           ▼
┌──────────────────────┐
│ External 5V Supply   │
│      (Servos)        │
└──────────────────────┘
# Hardware

## Required Components

- ELEGOO Arduino-compatible Nano with USB-C
- Adafruit Mini Pan-Tilt Kit with assembled micro servos
- KY-008 650 nm laser module
- USB-C cable
- Jumper wires
- External regulated 5V power supply for the servos
- Small breadboard — recommended, but optional