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

```
---
# Hardware

## Required Components
[Parts List](https://github.com/Brian-Walsh-0396/fun-stuff/blob/main/Arudino%20Laser/Parts_List.md)
- ELEGOO Arduino-compatible Nano with USB-C
- Adafruit Mini Pan-Tilt Kit with assembled micro servos
- KY-008 650 nm laser module
- USB-C cable
- Jumper wires
- External regulated 5V power supply for the servos
- Small breadboard — recommended, but optional
---
# Do I Need a Breadboard?

**No. A breadboard is not strictly required.**

Because the ELEGOO Nano has pre-soldered pins, individual components can be connected directly using jumper wires.

However, a small breadboard is **recommended** because it makes it much easier to distribute power and ground between the Nano, servos, laser module, and external power supply.

## Without a Breadboard

You can use point-to-point wiring:

```text
Nano D9  ─────────► Pan Servo Signal
Nano D10 ─────────► Tilt Servo Signal
Nano D7  ─────────► Laser Signal

Nano GND ─────────► Common Ground
External 5V ──────► Servo Power
External GND ─────► Common Ground
```
This works, but the wiring can become difficult to manage.
## With a Breadboard
``` text
A breadboard provides convenient power and ground rails:
                    ELEGOO NANO
                  ┌─────────────┐
                  │             │
             D9 ─┼─────────────┼──► Pan Servo Signal
            D10 ─┼─────────────┼──► Tilt Servo Signal
             D7 ─┼─────────────┼──► Laser Signal
            GND ─┼─────────────┼───────┐
                  └─────────────┘       │
                                        │
                                        ▼
                              ┌─────────────────┐
                              │    Breadboard   │
                              │                 │
External 5V ─────────────────►│ + Power Rail   │
External GND ────────────────►│ - Ground Rail  │
                              │                 │
                              └───────┬─────────┘
                                      │
                         ┌────────────┴────────────┐
                         ▼                         ▼
                    Pan Servo                Tilt Servo
                    5V / GND                 5V / GND

```
## Reccomdended Set Up
For this project, using a small breadboard is recommended. It provides a cleaner wiring layout and makes it easier to establish a common ground between the ELEGOO Nano, external 5V power supply, servos, and laser module.
> **Important:** The external 5V supply's ground must be connected to the Nano's GND so that the servo control signals have a common reference.
---
# Wiring

## Servo Connections

The two servos control the **pan** and **tilt** axes.

| Component         | Nano Pin |
| ----------------- | -------- |
| Pan Servo Signal  | D9       |
| Tilt Servo Signal | D10      |

## Pan Servo
``` text
Pan Servo
├── Signal ──► Nano D9
├── +5V ────► External 5V
└── GND ────► Common GND
```
## Tilt Servo
``` text
Pan Servo
├── Signal ──► Nano D9
├── +5V ────► External 5V
└── GND ────► Common GND
```
> **Important:** Connect the external power supply's GND to the Nano's GND. The Nano and external power supply need a common ground for the servo control signals to work correctly.
---
## Laser Module
The KY-008 module typically has three connections:
``` text
KY-008
├── S ──► Nano D7
├── + ──► 5V
└── - ──► GND
```
### Important

KY-008-style modules can vary between manufacturers.

Verify the labels on your actual module before connecting it.

Do not assume the pin order based solely on the appearance of the module.
---
## Complete Pin Out
| Nano Pin | Function           | Connected To        |
| -------- | ------------------ | ------------------- |
| D7       | Laser Control      | KY-008 Signal       |
| D9       | Pan Control        | Pan Servo Signal    |
| D10      | Tilt Control       | Tilt Servo Signal   |
| GND      | Common Ground      | External Supply GND |
| 5V       | Logic/Module Power | KY-008              |

The servos should receive their power from the external regulated 5V supply.