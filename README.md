# CH32V003 High(ish) Current DC Motor PWM Driver
A High(ish) Current (5A, Depending on PSU & MOSFET) DC Motor Driver, using the
CH32V003F4P6 (TSSOP-20)  
This Project utilises the CH32V003s Op-Amp, Multiple ADC Pins in Polled mode,
and PWM Output using TIM2. **TODO** Display Code.  
The code is written using a custom GPIO Library, and the other code is written
carefully so that it can serve as example code, and be used as a jumping-off
point for your own projects.

## Features (Goals)
* RMP Sense (Optinonal Add-On)
* RMP Hold mode
* Current Sensing & Over-Current Protection
* Display for Mode, Current, PWM Percent, and RPM

**WORK IN PROGRESS**
## TODO
* Op-Amp Testing & Implimentation
* Choose and create driver for display

----
ADBeta (c) 2024
