# Assistive Vest Prototype

An Arduino-based sensor-integration prototype for obstacle, surface-change, temperature, and fall-event alerts. It combines ultrasonic, infrared, temperature, and accelerometer input with a buzzer and RGB status LED.

**Stack:** Arduino Uno · C++ / Arduino APIs · I²C · MPU6050 · HC-SR04 · DHT11

## Sensor behavior

| Input | Implemented response |
| --- | --- |
| Ultrasonic distance below 100 cm | Slow alert tone |
| Distance below 50 cm | Faster alert tone |
| Distance below 20 cm | Continuous higher-pitched tone |
| IR input high | Rapid alert tone and red LED channel |
| Temperature above 30°C | Distinct tone and blue LED channel |
| Low acceleration followed by high acceleration | Alternating red/yellow LED sequence |

The green LED channel represents the ultrasonic alert state. LED channels can combine. Buzzer handling prioritizes IR, then temperature, then ultrasonic alerts.

## Wiring reference

The assignments below come directly from [code.c](code.c).

| Component | Arduino connection |
| --- | --- |
| RGB LED red / green / blue | D3 / D5 / D11 |
| IR sensor output | D7 |
| Buzzer | D8 |
| HC-SR04 trigger / echo | D9 / D10 |
| DHT11 data | D6 |
| MPU6050 | I²C address `0x68` via Wire |

Use the board's I²C pins for the MPU6050. Power, common ground, LED resistors, and module voltage requirements must match the actual hardware.

## Build and upload

1. Open Arduino IDE and create a new sketch.
2. Copy the contents of [code.c](code.c) into the sketch's `.ino` file. Despite its repository filename, this is Arduino C++ source, not a standalone C program.
3. Install the DHT library providing `DHT.h` and any dependencies it requests. `Wire` is supplied by the Arduino platform.
4. Select the Arduino Uno board and connected port, verify, and upload.
5. Open the Serial Monitor at **9600 baud** to inspect acceleration, distance, temperature, and IR readings.

## Design notes

- Ultrasonic reads use a timeout and report `-1` if no echo is received.
- Temperature alerts ignore NaN readings.
- The fall heuristic uses raw acceleration thresholds and a four-second cooldown.
- Blocking delays simplify alert sequencing but reduce sensor update responsiveness.

## Scope

An academic hardware prototype. The source does not establish reliable real-world fall detection or navigation assistance; it needs hardware calibration, wiring documentation, and measured validation before those claims can be made.

## License

See [LICENSE](LICENSE).
