# Measuring Temperature with Olimex MOD-TC-MK2-31855 and a Raspberry Pi

This repository is a work in progress trying to measure temperature with a thermocouple (type K) connected to a [Raspberry Pi](http://www.raspberrypi.org) using a [Olimex MOD-TC-MK2-31855](https://www.olimex.com/Products/Modules/Sensors/MOD-TC-MK2-31855).

**ATTENTION:** This is a _work in progress_ and currently not functioning propperly. It is on github for reference and discussion.

## Hardware Requirements

 * [Raspberry Pi Model B/rev. 2.0](http://www.raspberrypi.org/product/model-b/) (other models have not been tested but might also work)
 * Olimex [Olimex MOD-TC-MK2-31855](https://www.olimex.com/Products/Modules/Sensors/MOD-TC-MK2-31855)
 
### Wiring

Conneting the MOD-TC-MK2's UEXT interface with the Raspberry Pi's [GPIO ports](http://pi.gadgetoid.com/pinout) 
```
MOD-TC          Raspi
  1 --- 3.3 V --- 1
  2 ---  GND  --- 9
  5 ---  SCL  --- 5
  6 ---  SDA  --- 3
```

## Software Requirements

### I2C support

 * you might need to follow the Adafruit's [I2C configuration tutorial](https://learn.adafruit.com/adafruits-raspberry-pi-lesson-4-gpio-setup/configuring-i2c)
 
When installed and wired propperly `i2cdetect` should find the MOD-TC-MK2 on address 0x23 (or 0x48 for older firmware versions):

```
$ sudo i2cdetect -y 1
     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f
00:          -- -- -- -- -- -- -- -- -- -- -- -- -- 
10: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
20: -- -- -- 23 -- -- -- -- -- -- -- -- -- -- -- -- 
30: -- -- -- -- -- -- -- -- -- -- -- UU -- -- -- -- 
40: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
50: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
60: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
70: -- -- -- -- -- -- -- --                         
```

If your MOD-TC-MK2-31855 shows up on address 0x48 you have to edit `mod_tc_read_temp.c` and change the address constant from `static int I2C_ADDR = 0x23;` to `static int I2C_ADDR = 0x48;`

### wiringPi

To compile you need to [install](http://wiringpi.com/download-and-install/) [wiringPi](http://wiringpi.com) - be sure to also read the [I2C Library](https://projects.drogon.net/raspberry-pi/wiringpi/i2c-library/) information.
 * this code is based on Olimex OLINUXINO code at https://github.com/OLIMEX/OLINUXINO/tree/master/SOFTWARE/A13/MOD-TC-MK2
 
## Building the Software

With *wiringPi* installed just run `make` and run the resulting executable:

```
$ sudo ./mod-tc-read-temp 
raw data (4 bytes): 80 9D B0 81
INTERNAL TEMPERATURE: -98.5000 C
EXTERNAL TEMPERATURE: -2021.00 C
```

##Bugs

Temperature readings do not show reasonable values (see above).

