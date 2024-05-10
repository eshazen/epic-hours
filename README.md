# epic-hours

This is a project to create an electronic sign to display the opening
hours in EPIC.  Basic function is simple:  display open and close time
for each day of the week

There are (14) 8x24 matrix displays for open/close 7 days/week.
These use a special font to fit in the space:

    ten hours:   (blank) or 1
    one hours:   (blank) or 0-9
    ten minutes: (blank) or 0, 3
    one minutes: (blank) or 0

At the top there is a larger matrix, 8x64 which could display
a scrolling banner.

Each 8x8 matrix is driven by a MAX7219 controller IC.
See for example 
[documentation](https://lastminuteengineers.com/max7219-dot-matrix-arduino-tutorial/).

### LED version

![Preview pixture](Sketches/preview.jpg)

The first version should just be "boring" LEDs.  Probably LED matrix,
at least for a banner at the top so messages can be scrolled.
8x8 maxtrix are very popular, so thinking of a 5x7 font.
24 columns work for 12:34 displays.

Need a total of 42+8 = 50 of these.  This is about $100
for modules with MAX7219 included.

Power, 3 digits, quiescent (blanked) = 20mA.

| Intensity | mA  |
|-----------|-----|
| 3         | 154 |
| 2         | 118 |
| 1         | 80  |

Say 17 groups of 3 @ 150mA = 2.6A @ 5V

Standoffs:

    8   8-32 x 1 inch
	200 6-32 x 1/2 inch less than 0.22" OD

[Preview](Sketches/amazon_8x8.jpg)

### Development Notes

A pcb [multi_display_shield] https://github.com/eshazen/epic-hours/tree/main/KiCAD/multi_display_shield provides power distribution and control for 8
displays.  Each day is organized as a 8x48 single display,
with an 8x64 banner at the top.

For Wifi:  propose to use an ESP32 30-pin dev board.  
See (this page)[https://lastminuteengineers.com/esp32-pinout-reference/]
for some very useful notes.

### Software / Interface

Use Arduino Uno as display controller.  Proposed (serial) command set:

	H            - Help - list commands
	T n hh:mm    - Time - set weekday time (0,1 = Mon, 2,3 = Tue etc)
	B text...    - Banner - set banner text.  Scroll if too long
    I n val      - Intensity - set intensity n=0-7

Could use either USB/serial connection or direct wire to 0/1 or 
other pins using SoftwareSerial.
	
Use an ESP32 or other to connect to WiFi and retrieve commands
from somewhere.


	
