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
Copyright (c) ADBeta 2024

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 2 of the License, 
or (at your option) any later version.

This program is distributed in the hope that it will be useful, but 
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  
See the GNU General Public License for more details.
