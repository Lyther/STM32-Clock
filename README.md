# STM32-Clock

This project is already well done, but I still want to add some functions that are cool! All the objects are already compiled. Use serial download tool to download ./OBJ/TEST.hex into board. If you find any bugs, please raise an issue.

Use STM32F103RC to realize digital clock, implement the following functions:
* Show both digital clock and circle clock, which would in the same panel.
* Time upadte and refresh by second.
* Set time and date by setting panel, using touch screen.
* Set language of the panels.
* Set alarm time and date.
* Use command to set time and date.
* Wake up LED light when alarm time reach.
* Realize counting down timer.
* Welcome message (or instructions) shown on main panel.

The following functions are in plan but not realize:
* GUI optimization.

----

## How to Use

Write executable file object into STM32F103 chip, when the system initialized, use the following keys to do:

1. clock panel:
	* key wake_up to back.
	* key 1 goes into setting panel.

2. setting panel:
	* key wake_up to back.

3. main panel (DIO panel):
	* key 1 goes into clock panel.
	* key 0 goes into setting panel.