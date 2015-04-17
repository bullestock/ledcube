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
EELAYER 24 0
EELAYER END
$Descr A4 11693 8268
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
L CONN_3 CONN1
U 1 1 552FFDE8
P 1800 1800
F 0 "CONN1" V 1750 1800 50  0000 C CNN
F 1 "IN" V 1850 1800 40  0000 C CNN
F 2 "" H 1800 1800 60  0000 C CNN
F 3 "" H 1800 1800 60  0000 C CNN
	1    1800 1800
	-1   0    0    -1  
$EndComp
$Comp
L CONN_3 CONN2
U 1 1 552FFDFC
P 5250 1800
F 0 "CONN2" V 5200 1800 50  0000 C CNN
F 1 "OUT" V 5300 1800 40  0000 C CNN
F 2 "" H 5250 1800 60  0000 C CNN
F 3 "" H 5250 1800 60  0000 C CNN
	1    5250 1800
	1    0    0    -1  
$EndComp
$Comp
L FUSE F1
U 1 1 552FFE46
P 2400 1700
F 0 "F1" H 2500 1750 40  0000 C CNN
F 1 "FUSE" H 2300 1650 40  0000 C CNN
F 2 "" H 2400 1700 60  0000 C CNN
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
F 2 "" V 2580 2750 30  0000 C CNN
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
F 2 "" V 2580 3250 30  0000 C CNN
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
F 2 "" V 2980 2250 30  0000 C CNN
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
F 2 "" V 3230 2500 30  0000 C CNN
F 3 "" H 3300 2500 30  0000 C CNN
	1    3300 2500
	0    1    1    0   
$EndComp
$Comp
L R R5
U 1 1 552FFF0A
P 4150 3250
F 0 "R5" V 4230 3250 40  0000 C CNN
F 1 "1K" V 4157 3251 40  0000 C CNN
F 2 "" V 4080 3250 30  0000 C CNN
F 3 "" H 4150 3250 30  0000 C CNN
	1    4150 3250
	1    0    0    -1  
$EndComp
$Comp
L PNP T1
U 1 1 552FFF1E
P 3750 2500
F 0 "T1" H 3750 2350 60  0000 R CNN
F 1 "PNP" H 3750 2650 60  0000 R CNN
F 2 "" H 3750 2500 60  0000 C CNN
F 3 "" H 3750 2500 60  0000 C CNN
	1    3750 2500
	1    0    0    -1  
$EndComp
$Comp
L PNP T2
U 1 1 552FFF32
P 4050 2300
F 0 "T2" H 4050 2150 60  0000 R CNN
F 1 "PNP" H 4050 2450 60  0000 R CNN
F 2 "" H 4050 2300 60  0000 C CNN
F 3 "" H 4050 2300 60  0000 C CNN
	1    4050 2300
	1    0    0    -1  
$EndComp
$Comp
L SCR T3
U 1 1 552FFF46
P 4500 2600
F 0 "T3" H 4650 2850 70  0000 C CNN
F 1 "TYN1225" H 4650 2250 70  0000 C CNN
F 2 "" H 4500 2600 60  0000 C CNN
F 3 "" H 4500 2600 60  0000 C CNN
	1    4500 2600
	1    0    0    -1  
$EndComp
$Comp
L TL431LP IC1
U 1 1 552FFF5A
P 3050 3000
F 0 "IC1" H 2800 3100 70  0000 C CNN
F 1 "TL431LP" H 2700 2900 70  0000 C CNN
F 2 "" H 3050 3000 60  0000 C CNN
F 3 "" H 3050 3000 60  0000 C CNN
	1    3050 3000
	-1   0    0    -1  
$EndComp
Wire Wire Line
	2150 1900 2150 3500
Wire Wire Line
	2150 3500 4900 3500
Wire Wire Line
	2650 1700 4900 1700
Wire Wire Line
	4900 3500 4900 1900
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
Wire Wire Line
	4150 2500 4150 3000
Connection ~ 4150 3000
Wire Wire Line
	4500 3150 4500 3500
Connection ~ 4500 3500
Wire Wire Line
	4500 2200 4500 1700
Connection ~ 4500 1700
$Comp
L POT P1
U 1 1 5530014F
P 2650 2250
F 0 "P1" V 2650 2150 50  0000 C CNN
F 1 "POT" H 2650 2250 50  0000 C CNN
F 2 "" H 2650 2250 60  0000 C CNN
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
NoConn ~ 2150 1800
NoConn ~ 4900 1800
$EndSCHEMATC
