# CEClient
STM32 Arduino library for HDMI CEC communication

Based on work of https://github.com/arpruss/ArduinoLib_CEClient<br>
Which was based on https://github.com/lucadentella/ArduinoLib_CEClient<br>
Which was based on https://github.com/floe/CEC

### What's different about this fork?

I wanted to get HDMI CEC working on STM32 blue pill boards under STM32 Arduino core:
https://github.com/stm32duino/Arduino_Core_STM32

A few changes were made for pin mode, but also force STM32 defines when using Arduino IDE.
I'm unsure why the defines were not being set in the environment automatically, but as I will eventually move my project under platformio (if I get serious about doing something with it), I didn't care to figure out why.

One of the major issues I ran into was the lack of examples and documentation of the library for writing CEC frames.
After stepping through the debugger for a few hours, I figured out the basics of what the library was expecting on the CEC line and in initialization/logic.
I've created some examples of writing power-on and HDMI-source-change frames.

If I get a chance I'll clean all this up a bit more.
However, this project is just for low-level testing of HDMI-CEC.

I've been trying to get CEC working on higher-level stacks such as ESP32 home and circuitpython without success.
Now that I have working code and circuit, I can better troubleshoot those other stacks.

## Helpful resources:

Nice pinout diagram for bluepill boards:
* http://land-boards.com/blwiki/images/a/a4/STM32F103C8T6-Blue-Pill-Pin-Layout.gif

A must for CEC frame decoding:
* https://www.cec-o-matic.com/

## Helpful references for CEC implementation:
Other hobbyist CEC circuit implementations are rather complex for some reason, I think it because they include level shifting assuming 5v arduino pins? I don't know, because I've never used a classic arduino MCU.

In any case, if you are using an MCU with 3.3v outputs that can do open-drain communication (I believe that is just tristate logic), then all is needed is an 27K pullup and a logic-level mosfet to not disrupt the CEC line with the MCU is off.

See Figure 329. HDMI-CEC block diagram:<br>
https://www.st.com/resource/en/reference_manual/rm0091-stm32f0x1stm32f0x2stm32f0x8-advanced-armbased-32bit-mcus-stmicroelectronics.pdf

Also of note, a number of STM32 models include hardware support for CEC such as the STM32F042K6T6.
This is not a bad choice as a NUCLEO-F042K6 board is about $10 and, includes an STLink/V2 programmer onboard, and it seems to have pin compatibility with arduino nano. For something more serious, this is probably the way to go.

I don't have one yet, but the following application notes include details on how to use the CEC function:
https://www.st.com/resource/en/application_note/an4066-developing-an-hdmicec-network-using-an-stm32f0xx-microcontroller-stmicroelectronics.pdf

https://www.st.com/en/evaluation-tools/nucleo-f042k6.html

## Failed attempts:
I tried other approaches before finally getting this project to work on STM32.
One promising source was an I2C to CEC bridge running on ATtiny10:
https://github.com/ConsciousCode/i2cec

I couldn't get that working either. It just sat there with the line high. It may have been my electrical connection though, as I found the STM32 application notes later. So I may try this again. It may be a cheap and quick way to get CEC (though figuring out how to program these things was also a hassle).

There is also one for Attiny25, but I did not try this one:<br>
https://github.com/SzymonSlupik/CEC-Tiny

I also tried an Atiny85 using the Arduino CEClient that this repo inherits, but again the line just sat high in my setup. https://www.arduino.cc/reference/en/libraries/ceclient/
