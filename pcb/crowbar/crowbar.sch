EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:special
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:crowbar-cache
EELAYER 25 0
EELAYER END
$Descr User 7874 5906
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L FUSE F1
U 1 1 552FFE46
P 2400 1700
F 0 "F1" H 2500 1750 40  0000 C CNN
F 1 "FUSE" H 2300 1650 40  0000 C CNN
F 2 "Fuse_Holders_and_Fuses:Fuseholder5x20_horiz_open_universal_Type-III" H 2400 1700 60  0001 C CNN
F 3 "" H 2400 1700 60  0000 C CNN
	1    2400 1700
	1    0    0    -1  
$EndComp
$Comp
L R R1
U 1 1 552FFEBA
P 2650 2750
F 0 "R1" H 2750 2750 40  0000 C CNN
F 1 "8K2" V 2657 2751 40  0000 C CNN
F 2 "Resistors_ThroughHole:Resistor_Vertical_RM5mm" V 2580 2750 30  0001 C CNN
F 3 "" H 2650 2750 30  0000 C CNN
	1    2650 2750
	-1   0    0    1   
$EndComp
$Comp
L R R2
U 1 1 552FFECE
P 2650 3250
F 0 "R2" H 2550 3250 40  0000 C CNN
F 1 "8K2" V 2657 3251 40  0000 C CNN
F 2 "Resistors_ThroughHole:Resistor_Vertical_RM5mm" V 2580 3250 30  0001 C CNN
F 3 "" H 2650 3250 30  0000 C CNN
	1    2650 3250
	1    0    0    -1  
$EndComp
$Comp
L R R3
U 1 1 552FFEE2
P 3050 2250
F 0 "R3" H 2950 2350 40  0000 C CNN
F 1 "2K2" V 3057 2251 40  0000 C CNN
F 2 "Resistors_ThroughHole:Resistor_Vertical_RM5mm" V 2980 2250 30  0001 C CNN
F 3 "" H 3050 2250 30  0000 C CNN
	1    3050 2250
	1    0    0    -1  
$EndComp
$Comp
L R R4
U 1 1 552FFEF6
P 3300 2500
F 0 "R4" V 3380 2500 40  0000 C CNN
F 1 "1K" V 3307 2501 40  0000 C CNN
F 2 "Resistors_ThroughHole:Resistor_Vertical_RM5mm" V 3230 2500 30  0001 C CNN
F 3 "" H 3300 2500 30  0000 C CNN
	1    3300 2500
	0    1    1    0   
$EndComp
$Comp
L R R5
U 1 1 552FFF0A
P 4150 3050
F 0 "R5" V 4230 3050 40  0000 C CNN
F 1 "1K" V 4157 3051 40  0000 C CNN
F 2 "Resistors_ThroughHole:Resistor_Vertical_RM5mm" V 4080 3050 30  0001 C CNN
F 3 "" H 4150 3050 30  0000 C CNN
	1    4150 3050
	1    0    0    -1  
$EndComp
$Comp
L PNP T1
U 1 1 552FFF1E
P 3750 2500
F 0 "T1" H 3750 2350 60  0000 R CNN
F 1 "PNP" H 3750 2650 60  0000 R CNN
F 2 "Housings_TO-92:TO-92_Molded_Narrow" H 3750 2500 60  0001 C CNN
F 3 "" H 3750 2500 60  0000 C CNN
	1    3750 2500
	1    0    0    1   
$EndComp
$Comp
L PNP T2
U 1 1 552FFF32
P 4050 2300
F 0 "T2" H 4050 2150 60  0000 R CNN
F 1 "PNP" H 4050 2450 60  0000 R CNN
F 2 "Housings_TO-92:TO-92_Molded_Narrow" H 4050 2300 60  0001 C CNN
F 3 "" H 4050 2300 60  0000 C CNN
	1    4050 2300
	1    0    0    1   
$EndComp
$Comp
L SCR T3
U 1 1 552FFF46
P 5350 2100
F 0 "T3" H 5500 2350 70  0000 C CNN
F 1 "TYN1225" H 5500 1750 70  0000 C CNN
F 2 "Transistors_TO-220:TO-220_Neutral123_Vertical" H 5350 2100 60  0001 C CNN
F 3 "" H 5350 2100 60  0000 C CNN
	1    5350 2100
	1    0    0    -1  
$EndComp
$Comp
L TL431LP IC1
U 1 1 552FFF5A
P 3050 3000
F 0 "IC1" H 2800 3100 70  0000 C CNN
F 1 "TL431LP" H 2700 2900 70  0000 C CNN
F 2 "Housings_TO-92:TO-92_Molded_Narrow" H 3050 3000 60  0001 C CNN
F 3 "" H 3050 3000 60  0000 C CNN
	1    3050 3000
	-1   0    0    -1  
$EndComp
Wire Wire Line
	2150 1900 2150 3500
Wire Wire Line
	2150 3500 5600 3500
Wire Wire Line
	2650 1700 5600 1700
Connection ~ 2650 3500
Connection ~ 3050 3500
Connection ~ 4150 3500
Connection ~ 2650 1700
Wire Wire Line
	3050 1700 3050 2000
Connection ~ 3050 1700
Wire Wire Line
	4150 2100 4150 1700
Connection ~ 4150 1700
$Comp
L POT P1
U 1 1 5530014F
P 2650 2250
F 0 "P1" V 2650 2150 50  0000 C CNN
F 1 "POT" H 2650 2250 50  0000 C CNN
F 2 "Potentiometers:Potentiometer_Trimmer-Piher-PT15-h2-5_vertical" H 2650 2250 60  0001 C CNN
F 3 "" H 2650 2250 60  0000 C CNN
	1    2650 2250
	0    1    1    0   
$EndComp
Wire Wire Line
	2650 2000 2650 1700
Wire Wire Line
	2650 2000 2800 2000
Wire Wire Line
	2800 2000 2800 2250
Wire Wire Line
	3850 2700 3850 3500
Connection ~ 3850 3500
Connection ~ 3050 2500
$Comp
L CONN_1 Vcc1
U 1 1 55318F55
P 2000 1700
F 0 "Vcc1" H 2080 1700 40  0000 L CNN
F 1 "CONN_1" H 2000 1755 30  0001 C CNN
F 2 "Wire_Pads:SolderWirePad_single_1-2mmDrill" H 2000 1700 60  0001 C CNN
F 3 "" H 2000 1700 60  0000 C CNN
	1    2000 1700
	-1   0    0    1   
$EndComp
$Comp
L CONN_1 GND1
U 1 1 55318F84
P 2000 1900
F 0 "GND1" H 2080 1900 40  0000 L CNN
F 1 "CONN_1" H 2000 1955 30  0001 C CNN
F 2 "Wire_Pads:SolderWirePad_single_1-2mmDrill" H 2000 1900 60  0001 C CNN
F 3 "" H 2000 1900 60  0000 C CNN
	1    2000 1900
	-1   0    0    1   
$EndComp
$Comp
L CONN_1 Vcc2
U 1 1 553190C9
P 5750 1700
F 0 "Vcc2" H 5830 1700 40  0000 L CNN
F 1 "CONN_1" H 5750 1755 30  0001 C CNN
F 2 "Wire_Pads:SolderWirePad_single_1-2mmDrill" H 5750 1700 60  0001 C CNN
F 3 "" H 5750 1700 60  0000 C CNN
	1    5750 1700
	1    0    0    -1  
$EndComp
$Comp
L CONN_1 GND2
U 1 1 553190F4
P 5750 1900
F 0 "GND2" H 5830 1900 40  0000 L CNN
F 1 "CONN_1" H 5750 1955 30  0001 C CNN
F 2 "Wire_Pads:SolderWirePad_single_1-2mmDrill" H 5750 1900 60  0001 C CNN
F 3 "" H 5750 1900 60  0000 C CNN
	1    5750 1900
	1    0    0    -1  
$EndComp
Connection ~ 4150 2500
$Comp
L LED D1
U 1 1 5533973C
P 4500 3300
F 0 "D1" V 4500 3400 50  0000 C CNN
F 1 "LED" V 4600 3450 50  0000 C CNN
F 2 "LEDs:LED-3MM" H 4500 3300 60  0001 C CNN
F 3 "" H 4500 3300 60  0000 C CNN
	1    4500 3300
	0    1    1    0   
$EndComp
$Comp
L R R6
U 1 1 55339B52
P 4500 2750
F 0 "R6" V 4580 2750 40  0000 C CNN
F 1 "470" V 4507 2751 40  0000 C CNN
F 2 "Resistors_ThroughHole:Resistor_Vertical_RM5mm" V 4430 2750 30  0001 C CNN
F 3 "" H 4500 2750 30  0000 C CNN
	1    4500 2750
	1    0    0    -1  
$EndComp
Wire Wire Line
	4500 3000 4500 3100
Connection ~ 4500 3500
Connection ~ 4500 2500
Wire Wire Line
	5600 3500 5600 1900
Wire Wire Line
	5350 2650 5350 3500
Connection ~ 5350 3500
$Comp
L R R7
U 1 1 5533A0C1
P 4750 2500
F 0 "R7" V 4830 2500 40  0000 C CNN
F 1 "1K" V 4757 2501 40  0000 C CNN
F 2 "Resistors_ThroughHole:Resistor_Vertical_RM5mm" V 4680 2500 30  0001 C CNN
F 3 "" H 4750 2500 30  0000 C CNN
	1    4750 2500
	0    1    1    0   
$EndComp
Wire Wire Line
	4150 2500 4500 2500
Wire Wire Line
	4150 3300 4150 3500
Wire Wire Line
	4150 2500 4150 2800
Connection ~ 5350 1700
$Comp
L CONN_1 GND1A1
U 1 1 5533ACA0
P 2000 2100
F 0 "GND1A1" H 2080 2100 40  0000 L CNN
F 1 "CONN_1" H 2000 2155 30  0001 C CNN
F 2 "Wire_Pads:SolderWirePad_single_1-2mmDrill" H 2000 2100 60  0001 C CNN
F 3 "" H 2000 2100 60  0000 C CNN
	1    2000 2100
	-1   0    0    1   
$EndComp
$Comp
L CONN_1 GND2A1
U 1 1 5533ACE4
P 5750 2100
F 0 "GND2A1" H 5830 2100 40  0000 L CNN
F 1 "CONN_1" H 5750 2155 30  0001 C CNN
F 2 "Wire_Pads:SolderWirePad_single_1-2mmDrill" H 5750 2100 60  0001 C CNN
F 3 "" H 5750 2100 60  0000 C CNN
	1    5750 2100
	1    0    0    -1  
$EndComp
Connection ~ 5600 2100
Connection ~ 2150 2100
$EndSCHEMATC
