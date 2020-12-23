# PowerPuck-mini
2S Li-Ion power system for FPV goggles (allows the use of full capacity potential of li-ion cells). This is the second version whitch is much easier to build compaired to V1 (and is also much smaller).

# Features:
• 9V Step-Up from a 2S source <br>
• On/off button<br>
• Adjustable low-voltage alarm<br>
• LED power and capacity indicator<br>

<img src="https://raw.githubusercontent.com/skaman82/PowerPuck-mini/master/img/PP.jpg"/>
<img src="https://raw.githubusercontent.com/skaman82/PowerPuck-mini/master/img/pcbs.jpg"/>


# PARTS:
Notes: Voltage divider is currently designed for 10v max. <br>

## PCB
C1,C5: 		100nF C0603 Capacitor (2x)<br>
C2: 		10nF C0805 Capacitor (1x)<br>
C3: 		1uF C0805 Capacitor 25V (1x)<br>
C4: 		2.2uF C0805 Capacitor (1x)<br>
LS1: 		Speaker CSS-0575B-SMT (1x)<br>
Q1, Q2:		BC848 30V 100mA SOT23-3 Transistor (2x)<br>
Q3: 		IRLML6402 Power Mosfet (1x)<br>
R8, R9:	  5k6 R0603 Resistor (2x)<br>
R7, R5: 		10k R0603 Resistor (2x)<br>
S1: 		MOMENTARY-SWITCH SMD 5.2MM (1x)<br>
U1: 		WS2812-2020 LED (1x)<br>
U2: 		LP2985 5V LDO Reg (1x)<br>
•: 		ATTINY85 SOIC8 (1x)<br><br>

## Additional
+ Produced PCB (0.8-1mm thicknes required for the case)<br>
+ U3V12F9		Pololu 9V Step-Up<br>
+ Printed case parts (use PETG because of the heat from the step-up. TPU for the relief)<br>
+ ISP flashing tool (and SOIC8 clamp) like USBTinyISP<br>
+ 1.5m 5.5 x 2.1 DC cable like this: https://www.ebay.de/itm/0-50-1-00-1-50-m-USB-DC-Kabel-5-5-x-2-1-2-5-mm-Winkelstecker-CU-15855/293148509182 (cut the USB and replace it with your own connector)<br>
+ 2S li-ion battery pack. 3500mah 18650 will last about 3.5h, 5000mah 21700 will last about 5h. 18650 DIY battery case see the V1: https://github.com/skaman82/PowerPuck<br>

# ASSEMBLY
+ Solder all the SMD parts and the step-up to the PCB
+ Compile and flash the code with ArduinoIDE using an ISP programmer (use the right hardware package mentioned in the code)
+ Connect to a 2S power source and test if everything
+ Solder on the cables, put it in the pinted case and glue it together

# OPERATION
Connect the battery. The device is powered on and the LED is showing the battery status as follows: <br>
+ GREEN: voltage is 4.20 - 3.65v per cell (full)<br>
+ YELLOW: voltage is 3.64- 3.45v per cell (half)<br>
+ RED: voltage is 3.44 - alarmvoltage (one third)<br>
+ BLINKING RED and beeping: voltage is below alarm voltage<br>
<br>
- Press the button once to toggle between ON and OFF state.<br>
- Press the button for 2 sek. to enter the setup-menu<br>
<br>
SETUP MENU:<br>
Press the button to toggle between settings:<br>

• GREEN: voltage alarm at 3.35v per cell<br>
• YELLOW: voltage alarm at 3.25v per cell<br>
• RED: voltage alarm at at 3.15v per cell<br>
- Press the button for 2 sek. to save your alarm setting and exit the setup menu <br>
