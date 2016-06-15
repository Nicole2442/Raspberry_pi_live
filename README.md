# Nicole-makeblock
Makeblock robot project for logistics trial

The package consists of the software (firmware and source code), deployment instructions (structure assembly instruction and delopment instructions).

Note for unit module:

Servo:

1. Change the wire connection to change the turn direction of servo motor
2. Check the wire connection when motor is in unstable working state
3. "Turning around" and "lifting up" need more speed to get the same effects as "straight forward" or "lifting down"

Raspberry connect with makeblock:

1. Prepare Raspberry Pi: raspberry pi 2 Model B v1.1 and OS: https://www.raspberrypi.org/downloads/noobs/ and following the start instructions of Raspberry Pi: https://www.raspberrypi.org/help/noobs-setup/
    
    Note: when first boot pi and meet the "Coloured splash screen", try replacing the OS system with a known good one (Ref: http://elinux.org/R-Pi_Troubleshooting#Coloured_splash_screen)
2. Prepare makeblock firmware: Firmware_For_mBlock/orion_firmware
3. Prepare connector: Me Shield for Raspberry Pi or micro USB
    
    Note: in the following steps, micro USB is used (Ref: https://github.com/Makeblock-official/PythonForMegaPi)
4. Follow the instructions: 
    http://learn.makeblock.com/cn/me-shield-for-raspberry-pi/ 
    or
    https://github.com/Makeblock-official/PythonForMegaPi
5. bot.start() #if using usb cable, need to call bot.start('/dev/ttyACM0')
    
    Note: need to check "/dev/tty*" device, sometimes the device is recognized as '/dev/ttyUSB0'

6. Test according to the link: https://github.com/jafletch/makeblock-serial

    Primarily test for serial communication (USB cable) between Makeblock Orion and Raspberry Pi based on Python

    Note: 
    a. Serial data: 
    
    Raspberry Pi is wired with Makeblock Orion through USB port; 
    This USB is recognized as "/dev/ttyUSB0" in Raspberry Pi; 
    This USB is recognized as "Serial" in Makeblock Orion; 
    This USB cable is not for power supply
    
    b. Power supply:
    
    If motor is used, Makeblock Orion must need +9v power supply;
    Raspberry Pi need the micro USB or Me Shield to be powered (Raspberry Pi cannot be powered by the USB cable for serial data transmission)
    
    c. Firmware:
    
    Upload /serial_demo/firmware/firmware.ino (modified from jafletch's makeblock-serial)
    
    d. Python on Raspberry Pi:
    
    Change "ser = serial.Serial('/dev/ttyAMA0', 115200)" to the specific device (e.g. '/dev/ttyUSB0') in file rpi_serial.py;
    Run /serial_demo/demo_dcmotor.py (modified from jafletch's makeblock-serial)
    


