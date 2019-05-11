# usbkeyboard_b200
Arduino Leonardo based USB Keyboard controller for Bondwell B200 keyboard (or any other up to 11Column/8Row)

The code is configured for the german Bondwell B-200 KB 2.0 keyboard board out of an Highscreen B200 XT Laptop.
All keys are mapped correctly just the FN-Key is mapped as the right Alt as it's needed for several special characters on the german keyboard layout.

Keys can easily be remapped in the "keyCodes" array. The code should work with any other keyboard that does not need more columns or rows (in total 19 I/O Lines) as that's the limit of the user Arduino Leonardo board.

You can find the pinout and what pin is connected where on the Arduino in the PNG.
