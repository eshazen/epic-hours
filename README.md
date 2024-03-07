# epic-hours

This is a project to create an electronic sign to display the opening
hours in EPIC.  Basic function is simple:  display open and close time
for each day of the week.

Also, it should display the current date and time, just for fun.

For flexibility, should be able to display 4 digits:

    ten hours:   (blank) or 1
    one hours:   (blank) or 0-9
    ten minutes: (blank) or 0, 3
    one minutes: (blank) or 0

If we come up with a universal digit display (e.g. 7-segment) then
we could just use it everywhere.

A total of 28 digits plus ~10 for date/time are needed

It would be cool to have a mechanical display with electronic
controls.  Some ideas:

* 7-segment with white/black segments which toggle somehow
* flip cards like the old clocks
* rotating wheel with digits

## XY mechanism to flip segments

Here's a crazy idea:  build a big array of 7-segment digits which
require some mechanical intervention to flip the segments (pushing a
pin, magnetic activation, etc).  Then, construct a big XY mechanism
(H-bot, CoreXY or other) on the back which would (slowly) go through
and flip the segments as needed.  It would be fun to watch in action!

The hours would change rarely, but a clock/calendar display would
update once a minute and provide some action.

## Overall design

A reasonable size for the sign is 18" x 24".  Individual digits would
be around 0.85 x 1.5 inches.

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
