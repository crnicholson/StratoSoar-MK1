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
