<picture>
 <source media="(prefers-color-scheme: dark)" srcset="https://github.com/charlienicholson3/StratoSoar/blob/main/schematic.png?raw=true">
 <source media="(prefers-color-scheme: light)" srcset="https://github.com/charlienicholson3/StratoSoar/blob/main/schematic.png?raw=true">
 <img alt="YOUR-ALT-TEXT" src="YOUR-DEFAULT-IMAGE">
</picture>

# NOTE: This PCB is wrong. We had to change a wire. I suggest designing your own PCB.

# StratoSoar
My repo for the StratoSoar project. The LightAPRS code is just a variation of the original code. This system is made to recover unmanned high-altitude gliders, launched from weather balloons. It also has sensors that write to EEPROM, and the power for that system is controlled by the main MCU, which is the LightAPRS. For those who do not know, APRS is a form of HAM radio. You need to be a HAM radio operator to do it. For those who do not know, LightAPRS W-2 is a lightweight APRS transmitter: github.com/lightaprs/LightAPRS-W-2.0. The PCB takes anything from 20 to 6 volts as input and turns off the sensor and the camera after a certain altitude. It can also control the recording of the video. The schematic shows everything. The PCB breaks into three parts: top, middle, and bottom layers that connect through header pins.

# Acknowledgements
Thanks to Harish KC1SIO for the idea and initial work. Thanks to the NEWBS team for fostering the idea and having the high altitude project. Special thanks to Max W0MXX and Seth KC1PZY for funding and help during this project. Thanks to Bob K5TEC at NeSciTech for supplying a workspace and collaboration area. 

# Nerd info
We used a RunCam DVR with a 2N2222 transistor to close the circuit where the button is mounted. This took lots of development and didn't always go right. It highly depends on the voltage of the MCU and the amount of current your MCU can sink into the base pin of the transistor. Because of this, we had to not use the Serial Wombat and to solder a wire onto the LightAPRS. The SerialWombat can still be used to power the MOSFET, though it should be noted that the voltage goes through a series of transistor amplifiers to have enough voltage to turn on the FET. The camera on the DVR was a RunCam Atom.
