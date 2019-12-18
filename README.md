# STM32-Clock

Use STM32F103RC to realize digital clock, implement the following functions:
* Show both digital clock and circle clock, which would in the same panel.
* Time upadte and refresh by second.
* Set time and date by setting panel, using touch screen.
* Set language of the panels.
* Set alarm time and date.
* Use command to set time and date.
* Wake up LED light when alarm time reach.

The following functions are in plan but not realize:
* Realize counting down timer.
* GUI optimization.
* Welcome message (or instructions) shown on main panel.

----

## How to Use

Write executable file object into STM32F103 chip, when the system initialized, use the following keys to do:

1. clock panel:
	* key wake_up to back.
	* key 1 goes into setting panel.
	* key 0 initialize counting down timer.(TODO)

2. setting panel:
	* key wake_up to back.

3. main panel (may remove in the future):
	* key 1 goes into clock panel.
	* key 0 goes into setting panel.