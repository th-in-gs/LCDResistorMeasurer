EESchema Schematic File Version 4
LIBS:Ohmmeter-cache
EELAYER 29 0
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
L Device:Q_PNP_ECB Q5
U 1 1 5E10FAC8
P 8050 4950
F 0 "Q5" H 8241 4996 50  0000 L CNN
F 1 "KT361" H 8241 4905 50  0000 L CNN
F 2 "" H 8250 5050 50  0001 C CNN
F 3 "~" H 8050 4950 50  0001 C CNN
	1    8050 4950
	1    0    0    -1  
$EndComp
$Comp
L Device:Q_PNP_ECB Q3
U 1 1 5E110949
P 6550 4950
F 0 "Q3" H 6741 4996 50  0000 L CNN
F 1 "KT361" H 6741 4905 50  0000 L CNN
F 2 "" H 6750 5050 50  0001 C CNN
F 3 "~" H 6550 4950 50  0001 C CNN
	1    6550 4950
	1    0    0    -1  
$EndComp
$Comp
L Device:Q_PNP_ECB Q4
U 1 1 5E110FD9
P 7300 4950
F 0 "Q4" H 7491 4996 50  0000 L CNN
F 1 "KT361" H 7491 4905 50  0000 L CNN
F 2 "" H 7500 5050 50  0001 C CNN
F 3 "~" H 7300 4950 50  0001 C CNN
	1    7300 4950
	1    0    0    -1  
$EndComp
$Comp
L Device:Q_PNP_ECB Q1
U 1 1 5E111AA6
P 5050 4950
F 0 "Q1" H 5241 4996 50  0000 L CNN
F 1 "KT361" H 5241 4905 50  0000 L CNN
F 2 "" H 5250 5050 50  0001 C CNN
F 3 "~" H 5050 4950 50  0001 C CNN
	1    5050 4950
	1    0    0    -1  
$EndComp
$Comp
L Device:R R17
U 1 1 5E112800
P 5150 5900
F 0 "R17" H 5220 5946 50  0000 L CNN
F 1 "200" H 5220 5855 50  0000 L CNN
F 2 "" V 5080 5900 50  0001 C CNN
F 3 "~" H 5150 5900 50  0001 C CNN
	1    5150 5900
	1    0    0    -1  
$EndComp
$Comp
L Device:R R18
U 1 1 5E11391E
P 5900 5900
F 0 "R18" H 5970 5946 50  0000 L CNN
F 1 "2.11k" H 5970 5855 50  0000 L CNN
F 2 "" V 5830 5900 50  0001 C CNN
F 3 "~" H 5900 5900 50  0001 C CNN
	1    5900 5900
	1    0    0    -1  
$EndComp
$Comp
L Device:R R19
U 1 1 5E113F21
P 6650 5900
F 0 "R19" H 6720 5946 50  0000 L CNN
F 1 "20K" H 6720 5855 50  0000 L CNN
F 2 "" V 6580 5900 50  0001 C CNN
F 3 "~" H 6650 5900 50  0001 C CNN
	1    6650 5900
	1    0    0    -1  
$EndComp
$Comp
L Device:R R20
U 1 1 5E114428
P 7400 5900
F 0 "R20" H 7470 5946 50  0000 L CNN
F 1 "200k" H 7470 5855 50  0000 L CNN
F 2 "" V 7330 5900 50  0001 C CNN
F 3 "~" H 7400 5900 50  0001 C CNN
	1    7400 5900
	1    0    0    -1  
$EndComp
$Comp
L Device:R R21
U 1 1 5E114959
P 8150 5750
F 0 "R21" H 8220 5796 50  0000 L CNN
F 1 "1M" H 8220 5705 50  0000 L CNN
F 2 "" V 8080 5750 50  0001 C CNN
F 3 "~" H 8150 5750 50  0001 C CNN
	1    8150 5750
	1    0    0    -1  
$EndComp
$Comp
L Device:R R22
U 1 1 5E115071
P 8150 6050
F 0 "R22" H 8220 6096 50  0000 L CNN
F 1 "1M" H 8220 6005 50  0000 L CNN
F 2 "" V 8080 6050 50  0001 C CNN
F 3 "~" H 8150 6050 50  0001 C CNN
	1    8150 6050
	1    0    0    -1  
$EndComp
Wire Wire Line
	5150 6500 5900 6500
Connection ~ 5900 6500
Wire Wire Line
	5900 6500 6650 6500
Wire Wire Line
	6650 6500 7400 6500
Connection ~ 6650 6500
Connection ~ 7400 6500
Wire Wire Line
	7400 6500 8150 6500
$Comp
L Device:Jumper JP1
U 1 1 5E119219
P 6650 7000
F 0 "JP1" V 6604 7127 50  0000 L CNN
F 1 "Test Terminal" V 6695 7127 50  0000 L CNN
F 2 "" H 6650 7000 50  0001 C CNN
F 3 "~" H 6650 7000 50  0001 C CNN
	1    6650 7000
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6650 6500 6650 6700
$Comp
L Device:Q_PNP_ECB Q11
U 1 1 5E1342D7
P 7700 5450
F 0 "Q11" H 7891 5496 50  0000 L CNN
F 1 "KT361" H 7891 5405 50  0000 L CNN
F 2 "" H 7900 5550 50  0001 C CNN
F 3 "~" H 7700 5450 50  0001 C CNN
	1    7700 5450
	1    0    0    -1  
$EndComp
$Comp
L Device:Q_PNP_ECB Q9
U 1 1 5E1342EB
P 6200 5450
F 0 "Q9" H 6391 5496 50  0000 L CNN
F 1 "KT361" H 6391 5405 50  0000 L CNN
F 2 "" H 6400 5550 50  0001 C CNN
F 3 "~" H 6200 5450 50  0001 C CNN
	1    6200 5450
	1    0    0    -1  
$EndComp
$Comp
L Device:Q_PNP_ECB Q10
U 1 1 5E1342F5
P 6950 5450
F 0 "Q10" H 7141 5496 50  0000 L CNN
F 1 "KT361" H 7141 5405 50  0000 L CNN
F 2 "" H 7150 5550 50  0001 C CNN
F 3 "~" H 6950 5450 50  0001 C CNN
	1    6950 5450
	1    0    0    -1  
$EndComp
$Comp
L Device:Q_PNP_ECB Q7
U 1 1 5E1342FF
P 4700 5450
F 0 "Q7" H 4891 5496 50  0000 L CNN
F 1 "KT361" H 4891 5405 50  0000 L CNN
F 2 "" H 4900 5550 50  0001 C CNN
F 3 "~" H 4700 5450 50  0001 C CNN
	1    4700 5450
	1    0    0    -1  
$EndComp
$Comp
L Device:R R23
U 1 1 5E134309
P 4800 6100
F 0 "R23" H 4870 6146 50  0000 L CNN
F 1 "470" H 4870 6055 50  0000 L CNN
F 2 "" V 4730 6100 50  0001 C CNN
F 3 "~" H 4800 6100 50  0001 C CNN
	1    4800 6100
	1    0    0    -1  
$EndComp
$Comp
L Device:R R24
U 1 1 5E134313
P 5550 6100
F 0 "R24" H 5620 6146 50  0000 L CNN
F 1 "4.7k" H 5620 6055 50  0000 L CNN
F 2 "" V 5480 6100 50  0001 C CNN
F 3 "~" H 5550 6100 50  0001 C CNN
	1    5550 6100
	1    0    0    -1  
$EndComp
$Comp
L Device:R R25
U 1 1 5E13431D
P 6300 6100
F 0 "R25" H 6370 6146 50  0000 L CNN
F 1 "47k" H 6370 6055 50  0000 L CNN
F 2 "" V 6230 6100 50  0001 C CNN
F 3 "~" H 6300 6100 50  0001 C CNN
	1    6300 6100
	1    0    0    -1  
$EndComp
$Comp
L Device:R R26
U 1 1 5E134327
P 7050 6100
F 0 "R26" H 7120 6146 50  0000 L CNN
F 1 "470K" H 7120 6055 50  0000 L CNN
F 2 "" V 6980 6100 50  0001 C CNN
F 3 "~" H 7050 6100 50  0001 C CNN
	1    7050 6100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR035
U 1 1 5E143690
P 6650 7300
F 0 "#PWR035" H 6650 7050 50  0001 C CNN
F 1 "GND" H 6655 7127 50  0000 C CNN
F 2 "" H 6650 7300 50  0001 C CNN
F 3 "" H 6650 7300 50  0001 C CNN
	1    6650 7300
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR031
U 1 1 5E143FBD
P 5550 6250
F 0 "#PWR031" H 5550 6000 50  0001 C CNN
F 1 "GND" H 5555 6077 50  0000 C CNN
F 2 "" H 5550 6250 50  0001 C CNN
F 3 "" H 5550 6250 50  0001 C CNN
	1    5550 6250
	1    0    0    -1  
$EndComp
Wire Wire Line
	4500 5450 4500 4950
Wire Wire Line
	4500 4950 4850 4950
Wire Wire Line
	5250 5450 5250 5150
Wire Wire Line
	6000 5450 6000 5150
Wire Wire Line
	6000 5150 6350 5150
Wire Wire Line
	6350 5150 6350 4950
Wire Wire Line
	6750 5450 6750 5150
Wire Wire Line
	6750 5150 7100 5150
Wire Wire Line
	7100 5150 7100 4950
Wire Wire Line
	7500 5450 7500 5150
Wire Wire Line
	7500 5150 7850 5150
Wire Wire Line
	7850 5150 7850 4950
$Comp
L Device:R R27
U 1 1 5E13433B
P 7800 6100
F 0 "R27" H 7870 6146 50  0000 L CNN
F 1 "4.7M" H 7870 6055 50  0000 L CNN
F 2 "" V 7730 6100 50  0001 C CNN
F 3 "~" H 7800 6100 50  0001 C CNN
	1    7800 6100
	1    0    0    -1  
$EndComp
Wire Wire Line
	7800 5650 7800 5950
$Comp
L power:GND #PWR030
U 1 1 5E19D60A
P 4800 6250
F 0 "#PWR030" H 4800 6000 50  0001 C CNN
F 1 "GND" H 4805 6077 50  0000 C CNN
F 2 "" H 4800 6250 50  0001 C CNN
F 3 "" H 4800 6250 50  0001 C CNN
	1    4800 6250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR032
U 1 1 5E19D96E
P 6300 6250
F 0 "#PWR032" H 6300 6000 50  0001 C CNN
F 1 "GND" H 6305 6077 50  0000 C CNN
F 2 "" H 6300 6250 50  0001 C CNN
F 3 "" H 6300 6250 50  0001 C CNN
	1    6300 6250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR033
U 1 1 5E19DE7B
P 7050 6250
F 0 "#PWR033" H 7050 6000 50  0001 C CNN
F 1 "GND" H 7055 6077 50  0000 C CNN
F 2 "" H 7050 6250 50  0001 C CNN
F 3 "" H 7050 6250 50  0001 C CNN
	1    7050 6250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR034
U 1 1 5E19E300
P 7800 6250
F 0 "#PWR034" H 7800 6000 50  0001 C CNN
F 1 "GND" H 7805 6077 50  0000 C CNN
F 2 "" H 7800 6250 50  0001 C CNN
F 3 "" H 7800 6250 50  0001 C CNN
	1    7800 6250
	1    0    0    -1  
$EndComp
Wire Wire Line
	5150 6050 5150 6500
Wire Wire Line
	5900 6050 5900 6500
Wire Wire Line
	6650 6050 6650 6500
Wire Wire Line
	7400 6050 7400 6500
Wire Wire Line
	8150 6200 8150 6500
Wire Wire Line
	4800 5650 4800 5950
Wire Wire Line
	6300 5650 6300 5950
Wire Wire Line
	7050 5650 7050 5950
$Comp
L power:+3.3V #PWR022
U 1 1 5E1B5765
P 5150 4650
F 0 "#PWR022" H 5150 4500 50  0001 C CNN
F 1 "+3.3V" H 5165 4823 50  0000 C CNN
F 2 "" H 5150 4650 50  0001 C CNN
F 3 "" H 5150 4650 50  0001 C CNN
	1    5150 4650
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR023
U 1 1 5E1B6535
P 5900 4650
F 0 "#PWR023" H 5900 4500 50  0001 C CNN
F 1 "+3.3V" H 5915 4823 50  0000 C CNN
F 2 "" H 5900 4650 50  0001 C CNN
F 3 "" H 5900 4650 50  0001 C CNN
	1    5900 4650
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR024
U 1 1 5E1B6BF7
P 6650 4650
F 0 "#PWR024" H 6650 4500 50  0001 C CNN
F 1 "+3.3V" H 6665 4823 50  0000 C CNN
F 2 "" H 6650 4650 50  0001 C CNN
F 3 "" H 6650 4650 50  0001 C CNN
	1    6650 4650
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR025
U 1 1 5E1B722F
P 7400 4650
F 0 "#PWR025" H 7400 4500 50  0001 C CNN
F 1 "+3.3V" H 7415 4823 50  0000 C CNN
F 2 "" H 7400 4650 50  0001 C CNN
F 3 "" H 7400 4650 50  0001 C CNN
	1    7400 4650
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR026
U 1 1 5E1B76F7
P 8150 4650
F 0 "#PWR026" H 8150 4500 50  0001 C CNN
F 1 "+3.3V" H 8165 4823 50  0000 C CNN
F 2 "" H 8150 4650 50  0001 C CNN
F 3 "" H 8150 4650 50  0001 C CNN
	1    8150 4650
	1    0    0    -1  
$EndComp
$Comp
L MCU_Microchip_ATmega:ATmega8A-PU U3
U 1 1 5E1B8219
P 1850 3250
F 0 "U3" H 1850 3300 50  0000 C CNN
F 1 "ATmega8A-PU" H 1850 3150 50  0000 C CNN
F 2 "Package_DIP:DIP-28_W7.62mm" H 1850 3250 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/Microchip%208bit%20mcu%20AVR%20ATmega8A%20data%20sheet%2040001974A.pdf" H 1850 3250 50  0001 C CNN
	1    1850 3250
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C1
U 1 1 5E1E4BA0
P 2050 1700
F 0 "C1" V 1821 1700 50  0000 C CNN
F 1 "0.1uF" V 1912 1700 50  0000 C CNN
F 2 "" H 2050 1700 50  0001 C CNN
F 3 "~" H 2050 1700 50  0001 C CNN
	1    2050 1700
	0    1    1    0   
$EndComp
$Comp
L Device:C_Small C3
U 1 1 5E1E5629
P 1150 2850
F 0 "C3" H 1350 2850 50  0000 R CNN
F 1 "0.1uF" H 1450 2950 50  0000 R CNN
F 2 "" H 1150 2850 50  0001 C CNN
F 3 "~" H 1150 2850 50  0001 C CNN
	1    1150 2850
	-1   0    0    1   
$EndComp
Wire Wire Line
	1850 1700 1850 1850
Wire Wire Line
	1950 1700 1950 1850
$Comp
L power:GND #PWR08
U 1 1 5E1E8CA8
P 2150 1700
F 0 "#PWR08" H 2150 1450 50  0001 C CNN
F 1 "GND" H 2155 1527 50  0000 C CNN
F 2 "" H 2150 1700 50  0001 C CNN
F 3 "" H 2150 1700 50  0001 C CNN
	1    2150 1700
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR06
U 1 1 5E1E9D8C
P 1600 1350
F 0 "#PWR06" H 1600 1200 50  0001 C CNN
F 1 "+3.3V" H 1615 1523 50  0000 C CNN
F 2 "" H 1600 1350 50  0001 C CNN
F 3 "" H 1600 1350 50  0001 C CNN
	1    1600 1350
	1    0    0    -1  
$EndComp
Wire Wire Line
	1850 1700 1900 1700
Connection ~ 1950 1700
Connection ~ 1900 1700
Wire Wire Line
	1900 1700 1950 1700
Text GLabel 1050 2750 0    50   Input ~ 0
AREF
Wire Wire Line
	5900 5150 5900 5700
Wire Wire Line
	6650 5150 6650 5700
Wire Wire Line
	7400 5150 7400 5700
Wire Wire Line
	8150 5150 8150 5550
$Comp
L Device:Q_PNP_ECB Q6
U 1 1 5E1F8F08
P 8800 4950
F 0 "Q6" H 8991 4996 50  0000 L CNN
F 1 "KT361" H 8991 4905 50  0000 L CNN
F 2 "" H 9000 5050 50  0001 C CNN
F 3 "~" H 8800 4950 50  0001 C CNN
	1    8800 4950
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR027
U 1 1 5E1F9872
P 8900 4650
F 0 "#PWR027" H 8900 4500 50  0001 C CNN
F 1 "+3.3V" H 8915 4823 50  0000 C CNN
F 2 "" H 8900 4650 50  0001 C CNN
F 3 "" H 8900 4650 50  0001 C CNN
	1    8900 4650
	1    0    0    -1  
$EndComp
Wire Wire Line
	8900 5150 8900 5250
Wire Wire Line
	8900 5250 7800 5250
Wire Wire Line
	7800 5250 7050 5250
Connection ~ 7800 5250
Wire Wire Line
	7050 5250 6300 5250
Connection ~ 7050 5250
Wire Wire Line
	5250 5150 5600 5150
Wire Wire Line
	6300 5250 5550 5250
Connection ~ 6300 5250
Wire Wire Line
	5550 5250 4800 5250
Connection ~ 5550 5250
Text GLabel 9150 5250 2    50   Input ~ 0
AREF
Wire Wire Line
	9150 5250 8900 5250
Connection ~ 8900 5250
Wire Wire Line
	5600 5150 5600 4950
$Comp
L Device:Q_PNP_ECB Q2
U 1 1 5E11002E
P 5800 4950
F 0 "Q2" H 5991 4996 50  0000 L CNN
F 1 "KT361" H 5991 4905 50  0000 L CNN
F 2 "" H 6000 5050 50  0001 C CNN
F 3 "~" H 5800 4950 50  0001 C CNN
	1    5800 4950
	1    0    0    -1  
$EndComp
Wire Wire Line
	5150 4650 5150 4750
Wire Wire Line
	5900 4650 5900 4750
Wire Wire Line
	6650 4650 6650 4750
Wire Wire Line
	7400 4650 7400 4750
Wire Wire Line
	8150 4650 8150 4750
Wire Wire Line
	8900 4650 8900 4750
$Comp
L Device:R R16
U 1 1 5E2212F0
P 8950 4350
F 0 "R16" V 8743 4350 50  0000 C CNN
F 1 "680" V 8834 4350 50  0000 C CNN
F 2 "" V 8880 4350 50  0001 C CNN
F 3 "~" H 8950 4350 50  0001 C CNN
	1    8950 4350
	0    1    1    0   
$EndComp
Wire Wire Line
	8800 4350 8600 4350
$Comp
L power:GND #PWR021
U 1 1 5E223A9B
P 9200 4350
F 0 "#PWR021" H 9200 4100 50  0001 C CNN
F 1 "GND" H 9205 4177 50  0000 C CNN
F 2 "" H 9200 4350 50  0001 C CNN
F 3 "" H 9200 4350 50  0001 C CNN
	1    9200 4350
	1    0    0    -1  
$EndComp
Wire Wire Line
	9100 4350 9200 4350
$Comp
L Device:R R11
U 1 1 5E2272CB
P 4600 4350
F 0 "R11" V 4393 4350 50  0000 C CNN
F 1 "680" V 4484 4350 50  0000 C CNN
F 2 "" V 4530 4350 50  0001 C CNN
F 3 "~" H 4600 4350 50  0001 C CNN
	1    4600 4350
	0    1    1    0   
$EndComp
$Comp
L Device:R R12
U 1 1 5E2279F4
P 5350 4350
F 0 "R12" V 5143 4350 50  0000 C CNN
F 1 "680" V 5234 4350 50  0000 C CNN
F 2 "" V 5280 4350 50  0001 C CNN
F 3 "~" H 5350 4350 50  0001 C CNN
	1    5350 4350
	0    1    1    0   
$EndComp
$Comp
L Device:R R13
U 1 1 5E227F4F
P 6100 4350
F 0 "R13" V 5893 4350 50  0000 C CNN
F 1 "680" V 5984 4350 50  0000 C CNN
F 2 "" V 6030 4350 50  0001 C CNN
F 3 "~" H 6100 4350 50  0001 C CNN
	1    6100 4350
	0    1    1    0   
$EndComp
$Comp
L Device:R R14
U 1 1 5E228273
P 6850 4350
F 0 "R14" V 6643 4350 50  0000 C CNN
F 1 "680" V 6734 4350 50  0000 C CNN
F 2 "" V 6780 4350 50  0001 C CNN
F 3 "~" H 6850 4350 50  0001 C CNN
	1    6850 4350
	0    1    1    0   
$EndComp
$Comp
L Device:R R15
U 1 1 5E228654
P 7600 4350
F 0 "R15" V 7393 4350 50  0000 C CNN
F 1 "680" V 7484 4350 50  0000 C CNN
F 2 "" V 7530 4350 50  0001 C CNN
F 3 "~" H 7600 4350 50  0001 C CNN
	1    7600 4350
	0    1    1    0   
$EndComp
Wire Wire Line
	7850 4350 7750 4350
Wire Wire Line
	7100 4350 7000 4350
Wire Wire Line
	6350 4350 6250 4350
$Comp
L Device:Q_PNP_ECB Q8
U 1 1 5E1342E1
P 5450 5450
F 0 "Q8" H 5641 5496 50  0000 L CNN
F 1 "KT361" H 5641 5405 50  0000 L CNN
F 2 "" H 5650 5550 50  0001 C CNN
F 3 "~" H 5450 5450 50  0001 C CNN
	1    5450 5450
	1    0    0    -1  
$EndComp
Wire Wire Line
	5550 5650 5550 5950
Wire Wire Line
	5150 5150 5150 5700
Wire Wire Line
	5150 5700 5900 5700
Connection ~ 5150 5700
Wire Wire Line
	5150 5700 5150 5750
Connection ~ 5900 5700
Wire Wire Line
	5900 5700 5900 5750
Wire Wire Line
	5900 5700 6650 5700
Connection ~ 6650 5700
Wire Wire Line
	6650 5700 6650 5750
Wire Wire Line
	6650 5700 7400 5700
Connection ~ 7400 5700
Wire Wire Line
	7400 5700 7400 5750
Wire Wire Line
	7400 5700 8000 5700
Wire Wire Line
	8000 5700 8000 5550
Wire Wire Line
	8000 5550 8150 5550
Connection ~ 8150 5550
Wire Wire Line
	8150 5550 8150 5600
Text GLabel 9150 5550 2    50   Input ~ 0
ADC
Wire Wire Line
	7100 4350 7100 4950
Connection ~ 7100 4950
Wire Wire Line
	7850 4350 7850 4950
Connection ~ 7850 4950
Wire Wire Line
	8600 4350 8600 4950
Connection ~ 6350 4950
Wire Wire Line
	6350 4350 6350 4950
Text GLabel 2500 3350 2    50   Input ~ 0
ADC
Wire Wire Line
	2500 3350 2450 3350
$Comp
L power:GND #PWR019
U 1 1 5E2B80CD
P 1150 2950
F 0 "#PWR019" H 1150 2700 50  0001 C CNN
F 1 "GND" H 1155 2777 50  0000 C CNN
F 2 "" H 1150 2950 50  0001 C CNN
F 3 "" H 1150 2950 50  0001 C CNN
	1    1150 2950
	1    0    0    -1  
$EndComp
$Comp
L Device:R R10
U 1 1 5E2B8257
P 1250 1650
F 0 "R10" H 1320 1696 50  0000 L CNN
F 1 "10K" H 1320 1605 50  0000 L CNN
F 2 "" V 1180 1650 50  0001 C CNN
F 3 "~" H 1250 1650 50  0001 C CNN
	1    1250 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	1600 1350 1900 1350
Wire Wire Line
	1900 1350 1900 1700
Wire Wire Line
	1600 1350 1250 1350
Wire Wire Line
	1250 1350 1250 1500
Connection ~ 1600 1350
$Comp
L power:GND #PWR028
U 1 1 5E2D8709
P 1900 4800
F 0 "#PWR028" H 1900 4550 50  0001 C CNN
F 1 "GND" H 1905 4627 50  0000 C CNN
F 2 "" H 1900 4800 50  0001 C CNN
F 3 "" H 1900 4800 50  0001 C CNN
	1    1900 4800
	1    0    0    -1  
$EndComp
Wire Wire Line
	1850 4700 1850 4650
Wire Wire Line
	1900 4700 1900 4800
Wire Wire Line
	1850 4700 1900 4700
Wire Wire Line
	1950 4650 1950 4700
Wire Wire Line
	1950 4700 1900 4700
Connection ~ 1900 4700
Wire Wire Line
	1050 2750 1150 2750
Connection ~ 1150 2750
Wire Wire Line
	1150 2750 1250 2750
Wire Wire Line
	2450 3550 3700 3550
Wire Wire Line
	3700 3550 3700 1350
Wire Wire Line
	2450 3650 3750 3650
Wire Wire Line
	3750 3650 3750 1300
Wire Wire Line
	3800 3750 3800 1250
Wire Wire Line
	3850 3850 3850 1200
Wire Wire Line
	3300 3950 3300 3400
Wire Wire Line
	3350 5050 700  5050
Wire Wire Line
	700  5050 700  2350
Wire Wire Line
	700  2350 1250 2350
Wire Wire Line
	3350 3350 3350 5050
Wire Wire Line
	3400 3300 3400 5100
Wire Wire Line
	3400 5100 650  5100
Wire Wire Line
	650  5100 650  2550
Wire Wire Line
	650  2550 1250 2550
Wire Wire Line
	2450 2450 3400 2450
Wire Wire Line
	3400 2450 3400 2950
Wire Wire Line
	3350 2900 3350 2550
Wire Wire Line
	3350 2550 2450 2550
Wire Wire Line
	3300 2850 3300 2650
Wire Wire Line
	3300 2650 2450 2650
Wire Wire Line
	2450 3950 3300 3950
Wire Wire Line
	2450 3850 3850 3850
Wire Wire Line
	2450 3750 3800 3750
Wire Wire Line
	5150 2850 3300 2850
Wire Wire Line
	5150 2600 5150 2850
Wire Wire Line
	5250 2900 3350 2900
Wire Wire Line
	5250 2600 5250 2900
Wire Wire Line
	5350 2950 5350 2600
Wire Wire Line
	3400 2950 5350 2950
Wire Wire Line
	3450 2350 2450 2350
Wire Wire Line
	3450 3000 3450 2350
Wire Wire Line
	5450 3000 3450 3000
Wire Wire Line
	5450 2600 5450 3000
Wire Wire Line
	5550 3050 5550 2600
Wire Wire Line
	3500 3050 5550 3050
Wire Wire Line
	3500 2250 3500 3050
Wire Wire Line
	2450 2250 3500 2250
Wire Wire Line
	3550 2150 2450 2150
Wire Wire Line
	3550 3100 3550 2150
Wire Wire Line
	5650 3100 3550 3100
Wire Wire Line
	5650 2600 5650 3100
Wire Wire Line
	3550 4250 2450 4250
Wire Wire Line
	3550 3150 3550 4250
Wire Wire Line
	5750 3150 3550 3150
Wire Wire Line
	5750 2600 5750 3150
Wire Wire Line
	5850 3200 5850 2600
Wire Wire Line
	3500 3200 5850 3200
Wire Wire Line
	3500 4150 3500 3200
Wire Wire Line
	2450 4150 3500 4150
Wire Wire Line
	3450 4050 2450 4050
Wire Wire Line
	3450 3250 3450 4050
Wire Wire Line
	5950 3250 3450 3250
Wire Wire Line
	5950 2600 5950 3250
Wire Wire Line
	6050 3300 3400 3300
Wire Wire Line
	6050 2600 6050 3300
Wire Wire Line
	6150 3350 3350 3350
Wire Wire Line
	6150 2600 6150 3350
Wire Wire Line
	6250 3400 3300 3400
Wire Wire Line
	6250 2600 6250 3400
$Comp
L power:+3.3V #PWR04
U 1 1 5E1B0135
P 4800 850
F 0 "#PWR04" H 4800 700 50  0001 C CNN
F 1 "+3.3V" H 4815 1023 50  0000 C CNN
F 2 "" H 4800 850 50  0001 C CNN
F 3 "" H 4800 850 50  0001 C CNN
	1    4800 850 
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR03
U 1 1 5E1AFD74
P 4550 850
F 0 "#PWR03" H 4550 700 50  0001 C CNN
F 1 "+3.3V" H 4565 1023 50  0000 C CNN
F 2 "" H 4550 850 50  0001 C CNN
F 3 "" H 4550 850 50  0001 C CNN
	1    4550 850 
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR02
U 1 1 5E1AFB23
P 4300 850
F 0 "#PWR02" H 4300 700 50  0001 C CNN
F 1 "+3.3V" H 4315 1023 50  0000 C CNN
F 2 "" H 4300 850 50  0001 C CNN
F 3 "" H 4300 850 50  0001 C CNN
	1    4300 850 
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR01
U 1 1 5E1AF55C
P 4050 850
F 0 "#PWR01" H 4050 700 50  0001 C CNN
F 1 "+3.3V" H 4065 1023 50  0000 C CNN
F 2 "" H 4050 850 50  0001 C CNN
F 3 "" H 4050 850 50  0001 C CNN
	1    4050 850 
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR012
U 1 1 5E1AE8D4
P 4800 1700
F 0 "#PWR012" H 4800 1450 50  0001 C CNN
F 1 "GND" H 4805 1527 50  0000 C CNN
F 2 "" H 4800 1700 50  0001 C CNN
F 3 "" H 4800 1700 50  0001 C CNN
	1    4800 1700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR011
U 1 1 5E1AE5D6
P 4550 1700
F 0 "#PWR011" H 4550 1450 50  0001 C CNN
F 1 "GND" H 4555 1527 50  0000 C CNN
F 2 "" H 4550 1700 50  0001 C CNN
F 3 "" H 4550 1700 50  0001 C CNN
	1    4550 1700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR010
U 1 1 5E1AE2A5
P 4300 1700
F 0 "#PWR010" H 4300 1450 50  0001 C CNN
F 1 "GND" H 4305 1527 50  0000 C CNN
F 2 "" H 4300 1700 50  0001 C CNN
F 3 "" H 4300 1700 50  0001 C CNN
	1    4300 1700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR09
U 1 1 5E1ADF02
P 4050 1700
F 0 "#PWR09" H 4050 1450 50  0001 C CNN
F 1 "GND" H 4055 1527 50  0000 C CNN
F 2 "" H 4050 1700 50  0001 C CNN
F 3 "" H 4050 1700 50  0001 C CNN
	1    4050 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	4800 1200 5450 1200
Connection ~ 4800 1200
Wire Wire Line
	4550 1250 5350 1250
Connection ~ 4550 1250
Wire Wire Line
	4300 1300 5250 1300
Connection ~ 4300 1300
$Comp
L Device:R R2
U 1 1 5E11E0DB
P 4300 1000
F 0 "R2" H 4370 1046 50  0000 L CNN
F 1 "22K" H 4370 955 50  0000 L CNN
F 2 "" V 4230 1000 50  0001 C CNN
F 3 "~" H 4300 1000 50  0001 C CNN
	1    4300 1000
	1    0    0    -1  
$EndComp
$Comp
L Device:R R6
U 1 1 5E11E7DA
P 4300 1550
F 0 "R6" H 4370 1596 50  0000 L CNN
F 1 "22K" H 4370 1505 50  0000 L CNN
F 2 "" V 4230 1550 50  0001 C CNN
F 3 "~" H 4300 1550 50  0001 C CNN
	1    4300 1550
	1    0    0    -1  
$EndComp
$Comp
L Device:R R3
U 1 1 5E11EABF
P 4550 1000
F 0 "R3" H 4620 1046 50  0000 L CNN
F 1 "22K" H 4620 955 50  0000 L CNN
F 2 "" V 4480 1000 50  0001 C CNN
F 3 "~" H 4550 1000 50  0001 C CNN
	1    4550 1000
	1    0    0    -1  
$EndComp
$Comp
L Device:R R7
U 1 1 5E11F99C
P 4550 1550
F 0 "R7" H 4620 1596 50  0000 L CNN
F 1 "22K" H 4620 1505 50  0000 L CNN
F 2 "" V 4480 1550 50  0001 C CNN
F 3 "~" H 4550 1550 50  0001 C CNN
	1    4550 1550
	1    0    0    -1  
$EndComp
$Comp
L Device:R R4
U 1 1 5E11FD68
P 4800 1000
F 0 "R4" H 4870 1046 50  0000 L CNN
F 1 "22K" H 4870 955 50  0000 L CNN
F 2 "" V 4730 1000 50  0001 C CNN
F 3 "~" H 4800 1000 50  0001 C CNN
	1    4800 1000
	1    0    0    -1  
$EndComp
$Comp
L Device:R R8
U 1 1 5E120149
P 4800 1550
F 0 "R8" H 4870 1596 50  0000 L CNN
F 1 "22K" H 4870 1505 50  0000 L CNN
F 2 "" V 4730 1550 50  0001 C CNN
F 3 "~" H 4800 1550 50  0001 C CNN
	1    4800 1550
	1    0    0    -1  
$EndComp
Wire Wire Line
	5150 1350 4050 1350
Wire Wire Line
	4300 1400 4300 1300
Wire Wire Line
	4300 1150 4300 1300
Wire Wire Line
	4550 1150 4550 1250
Wire Wire Line
	4550 1250 4550 1400
Wire Wire Line
	4800 1150 4800 1200
Wire Wire Line
	3750 1300 4300 1300
Wire Wire Line
	3800 1250 4550 1250
Wire Wire Line
	3850 1200 4800 1200
Wire Wire Line
	4800 1400 4800 1200
Wire Wire Line
	3700 1350 4050 1350
Wire Wire Line
	5450 1200 5450 1350
Wire Wire Line
	5350 1250 5350 1350
Wire Wire Line
	5250 1300 5250 1350
Connection ~ 4050 1350
Wire Wire Line
	4050 1350 4050 1150
Wire Wire Line
	4050 1350 4050 1400
$Comp
L Device:R R5
U 1 1 5E11CB57
P 4050 1550
F 0 "R5" H 4120 1596 50  0000 L CNN
F 1 "22K" H 4120 1505 50  0000 L CNN
F 2 "" V 3980 1550 50  0001 C CNN
F 3 "~" H 4050 1550 50  0001 C CNN
	1    4050 1550
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 5E11C39F
P 4050 1000
F 0 "R1" H 4120 1046 50  0000 L CNN
F 1 "22K" H 4120 955 50  0000 L CNN
F 2 "" V 3980 1000 50  0001 C CNN
F 3 "~" H 4050 1000 50  0001 C CNN
	1    4050 1000
	1    0    0    -1  
$EndComp
$Comp
L Ohmmeter:DGL-0401YG-4EH DS1
U 1 1 5E114E28
P 5700 1100
F 0 "DS1" H 6378 271 50  0000 L CNN
F 1 "DGL-0401YG-4EH" H 6378 180 50  0000 L CNN
F 2 "" H 5700 1100 50  0001 C CNN
F 3 "" H 5700 1100 50  0001 C CNN
	1    5700 1100
	1    0    0    -1  
$EndComp
Wire Wire Line
	3150 4550 3150 2850
Wire Wire Line
	3150 2850 2450 2850
Wire Wire Line
	5200 4350 5200 3750
Wire Wire Line
	5200 3750 4350 3750
Wire Wire Line
	4350 3750 4350 4600
Wire Wire Line
	4350 4600 3100 4600
Wire Wire Line
	3100 4600 3100 2950
Wire Wire Line
	3100 2950 2450 2950
Wire Wire Line
	5950 4350 5950 3700
Wire Wire Line
	4300 3700 4300 4650
Wire Wire Line
	4300 4650 3050 4650
Wire Wire Line
	3050 4650 3050 3050
Wire Wire Line
	3050 3050 2450 3050
Wire Wire Line
	6700 4350 6700 3650
Wire Wire Line
	6700 3650 4250 3650
Wire Wire Line
	4250 3650 4250 4700
Wire Wire Line
	4250 4700 3000 4700
Wire Wire Line
	3000 4700 3000 3150
Wire Wire Line
	3000 3150 2450 3150
Wire Wire Line
	2450 3250 2950 3250
Wire Wire Line
	2950 3250 2950 4750
Wire Wire Line
	2950 4750 4200 4750
Wire Wire Line
	4200 4750 4200 3600
Wire Wire Line
	4200 3600 7450 3600
Wire Wire Line
	7450 3600 7450 4350
Wire Wire Line
	4450 4350 4400 4350
Wire Wire Line
	4400 4350 4400 4550
Wire Wire Line
	4400 4550 3150 4550
Wire Wire Line
	1250 2050 1250 2150
Connection ~ 1250 2050
Wire Wire Line
	1250 1800 1250 2050
$Comp
L power:GND #PWR015
U 1 1 5E1E95E1
P 1050 2050
F 0 "#PWR015" H 1050 1800 50  0001 C CNN
F 1 "GND" H 1055 1877 50  0000 C CNN
F 2 "" H 1050 2050 50  0001 C CNN
F 3 "" H 1050 2050 50  0001 C CNN
	1    1050 2050
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C2
U 1 1 5E1E3307
P 1150 2050
F 0 "C2" V 921 2050 50  0000 C CNN
F 1 "0.1uF" V 1012 2050 50  0000 C CNN
F 2 "" H 1150 2050 50  0001 C CNN
F 3 "~" H 1150 2050 50  0001 C CNN
	1    1150 2050
	0    1    1    0   
$EndComp
$Comp
L power:+5V #PWR05
U 1 1 5E81E6FE
P 6150 1200
F 0 "#PWR05" H 6150 1050 50  0001 C CNN
F 1 "+5V" H 6165 1373 50  0000 C CNN
F 2 "" H 6150 1200 50  0001 C CNN
F 3 "" H 6150 1200 50  0001 C CNN
	1    6150 1200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR07
U 1 1 5E81FA4E
P 6550 1350
F 0 "#PWR07" H 6550 1100 50  0001 C CNN
F 1 "GND" H 6555 1177 50  0000 C CNN
F 2 "" H 6550 1350 50  0001 C CNN
F 3 "" H 6550 1350 50  0001 C CNN
	1    6550 1350
	1    0    0    -1  
$EndComp
$Comp
L Device:R R9
U 1 1 5E820596
P 6400 1250
F 0 "R9" V 6193 1250 50  0000 C CNN
F 1 "47" V 6284 1250 50  0000 C CNN
F 2 "" V 6330 1250 50  0001 C CNN
F 3 "~" H 6400 1250 50  0001 C CNN
	1    6400 1250
	0    1    1    0   
$EndComp
Wire Wire Line
	6550 1250 6550 1350
Wire Wire Line
	6250 1250 6250 1350
Wire Wire Line
	6150 1200 6150 1350
$Comp
L power:+VDC #PWR018
U 1 1 5E8BC0E5
P 7150 2700
F 0 "#PWR018" H 7150 2600 50  0001 C CNN
F 1 "+VDC" H 7150 2975 50  0000 C CNN
F 2 "" H 7150 2700 50  0001 C CNN
F 3 "" H 7150 2700 50  0001 C CNN
	1    7150 2700
	1    0    0    -1  
$EndComp
$Comp
L Regulator_Linear:LP2950-3.3_TO92 U2
U 1 1 5E8BDB1F
P 9900 2700
F 0 "U2" H 9900 2942 50  0000 C CNN
F 1 "LP2950-3.3_TO92" H 9900 2851 50  0000 C CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 9900 2925 50  0001 C CIN
F 3 "http://www.ti.com/lit/ds/symlink/lp2950-n.pdf" H 9900 2650 50  0001 C CNN
	1    9900 2700
	1    0    0    -1  
$EndComp
$Comp
L Regulator_Linear:L78L05_TO92 U1
U 1 1 5E8C19ED
P 8100 2700
F 0 "U1" H 8100 2942 50  0000 C CNN
F 1 "L78L05_TO92" H 8100 2851 50  0000 C CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 8100 2925 50  0001 C CIN
F 3 "http://www.st.com/content/ccc/resource/technical/document/datasheet/15/55/e5/aa/23/5b/43/fd/CD00000446.pdf/files/CD00000446.pdf/jcr:content/translations/en.CD00000446.pdf" H 8100 2650 50  0001 C CNN
	1    8100 2700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR020
U 1 1 5E8C410A
P 8550 3250
F 0 "#PWR020" H 8550 3000 50  0001 C CNN
F 1 "GND" H 8555 3077 50  0000 C CNN
F 2 "" H 8550 3250 50  0001 C CNN
F 3 "" H 8550 3250 50  0001 C CNN
	1    8550 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	8100 3100 8100 3000
Wire Wire Line
	8550 3100 8550 3250
Wire Wire Line
	8550 3100 8450 3100
$Comp
L power:+5V #PWR016
U 1 1 5E8E26ED
P 8450 2600
F 0 "#PWR016" H 8450 2450 50  0001 C CNN
F 1 "+5V" H 8465 2773 50  0000 C CNN
F 2 "" H 8450 2600 50  0001 C CNN
F 3 "" H 8450 2600 50  0001 C CNN
	1    8450 2600
	1    0    0    -1  
$EndComp
Wire Wire Line
	8400 2700 8450 2700
Wire Wire Line
	8450 2700 8450 2600
Wire Wire Line
	10200 2700 10300 2700
Wire Wire Line
	7450 2700 7800 2700
Wire Wire Line
	7450 2700 7150 2700
Connection ~ 7450 2700
$Comp
L Device:C C4
U 1 1 5E92BC5C
P 7450 2900
F 0 "C4" H 7565 2946 50  0000 L CNN
F 1 "0.47uf" H 7565 2855 50  0000 L CNN
F 2 "" H 7488 2750 50  0001 C CNN
F 3 "~" H 7450 2900 50  0001 C CNN
	1    7450 2900
	1    0    0    -1  
$EndComp
Wire Wire Line
	7450 2700 7450 2750
Wire Wire Line
	7450 3050 7450 3100
Wire Wire Line
	7450 3100 8100 3100
Connection ~ 8100 3100
$Comp
L Device:C C5
U 1 1 5E967F65
P 8450 2900
F 0 "C5" H 8565 2946 50  0000 L CNN
F 1 "0.1uf" H 8565 2855 50  0000 L CNN
F 2 "" H 8488 2750 50  0001 C CNN
F 3 "~" H 8450 2900 50  0001 C CNN
	1    8450 2900
	1    0    0    -1  
$EndComp
Wire Wire Line
	8450 2700 8450 2750
Connection ~ 8450 2700
Wire Wire Line
	8450 3050 8450 3100
Connection ~ 8450 3100
Wire Wire Line
	8450 3100 8100 3100
Wire Wire Line
	9900 3000 9900 3100
Connection ~ 8550 3100
$Comp
L Device:C C7
U 1 1 5E9C2D53
P 10300 2900
F 0 "C7" H 10415 2946 50  0000 L CNN
F 1 "0.1uf" H 10415 2855 50  0000 L CNN
F 2 "" H 10338 2750 50  0001 C CNN
F 3 "~" H 10300 2900 50  0001 C CNN
	1    10300 2900
	1    0    0    -1  
$EndComp
Wire Wire Line
	9900 3100 10300 3100
Wire Wire Line
	10300 3100 10300 3050
Connection ~ 9900 3100
Wire Wire Line
	10300 2750 10300 2700
Connection ~ 10300 2700
Wire Wire Line
	7450 2300 7450 2700
Wire Wire Line
	9200 2300 7450 2300
Wire Wire Line
	10300 2700 10300 2600
$Comp
L power:+3.3V #PWR017
U 1 1 5E8E06C5
P 10300 2600
F 0 "#PWR017" H 10300 2450 50  0001 C CNN
F 1 "+3.3V" H 10315 2773 50  0000 C CNN
F 2 "" H 10300 2600 50  0001 C CNN
F 3 "" H 10300 2600 50  0001 C CNN
	1    10300 2600
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C8
U 1 1 5EA40191
P 10750 2900
F 0 "C8" H 10868 2946 50  0000 L CNN
F 1 "10uf" H 10868 2855 50  0000 L CNN
F 2 "" H 10788 2750 50  0001 C CNN
F 3 "~" H 10750 2900 50  0001 C CNN
	1    10750 2900
	1    0    0    -1  
$EndComp
Wire Wire Line
	10750 2750 10750 2700
Wire Wire Line
	10750 2700 10300 2700
Wire Wire Line
	10300 3100 10750 3100
Wire Wire Line
	10750 3100 10750 3050
Connection ~ 10300 3100
Wire Wire Line
	8550 3100 9350 3100
Wire Wire Line
	9600 2700 9350 2700
Wire Wire Line
	9350 2750 9350 2700
Connection ~ 9350 2700
Wire Wire Line
	9350 3050 9350 3100
Connection ~ 9350 3100
Wire Wire Line
	9350 3100 9900 3100
$Comp
L Device:CP C6
U 1 1 5EA3F415
P 9350 2900
F 0 "C6" H 9468 2946 50  0000 L CNN
F 1 "10uf" H 9468 2855 50  0000 L CNN
F 2 "" H 9388 2750 50  0001 C CNN
F 3 "~" H 9350 2900 50  0001 C CNN
	1    9350 2900
	1    0    0    -1  
$EndComp
Wire Wire Line
	9350 2700 9200 2700
Wire Wire Line
	9200 2700 9200 2300
$Comp
L power:GND #PWR013
U 1 1 5EBF6567
P 10150 1850
F 0 "#PWR013" H 10150 1600 50  0001 C CNN
F 1 "GND" H 10155 1677 50  0000 C CNN
F 2 "" H 10150 1850 50  0001 C CNN
F 3 "" H 10150 1850 50  0001 C CNN
	1    10150 1850
	1    0    0    -1  
$EndComp
$Comp
L power:+VDC #PWR014
U 1 1 5EBF7C1A
P 10500 1900
F 0 "#PWR014" H 10500 1800 50  0001 C CNN
F 1 "+VDC" H 10500 2175 50  0000 C CNN
F 2 "" H 10500 1900 50  0001 C CNN
F 3 "" H 10500 1900 50  0001 C CNN
	1    10500 1900
	1    0    0    -1  
$EndComp
$Comp
L power:PWR_FLAG #FLG02
U 1 1 5EBF8827
P 10500 1900
F 0 "#FLG02" H 10500 1975 50  0001 C CNN
F 1 "PWR_FLAG" H 10500 2073 50  0000 C CNN
F 2 "" H 10500 1900 50  0001 C CNN
F 3 "~" H 10500 1900 50  0001 C CNN
	1    10500 1900
	-1   0    0    1   
$EndComp
$Comp
L power:PWR_FLAG #FLG01
U 1 1 5EBF9EAB
P 10150 1850
F 0 "#FLG01" H 10150 1925 50  0001 C CNN
F 1 "PWR_FLAG" H 10150 2023 50  0000 C CNN
F 2 "" H 10150 1850 50  0001 C CNN
F 3 "~" H 10150 1850 50  0001 C CNN
	1    10150 1850
	1    0    0    -1  
$EndComp
$Comp
L Device:C C9
U 1 1 5EC42538
P 8750 5750
F 0 "C9" H 8865 5796 50  0000 L CNN
F 1 "0.01uf" H 8865 5705 50  0000 L CNN
F 2 "" H 8788 5600 50  0001 C CNN
F 3 "~" H 8750 5750 50  0001 C CNN
	1    8750 5750
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR029
U 1 1 5EC437D2
P 8750 6000
F 0 "#PWR029" H 8750 5750 50  0001 C CNN
F 1 "GND" H 8755 5827 50  0000 C CNN
F 2 "" H 8750 6000 50  0001 C CNN
F 3 "" H 8750 6000 50  0001 C CNN
	1    8750 6000
	1    0    0    -1  
$EndComp
Wire Wire Line
	8750 5900 8750 6000
Wire Wire Line
	8150 5550 8750 5550
Wire Wire Line
	8750 5600 8750 5550
Connection ~ 8750 5550
Wire Wire Line
	8750 5550 9150 5550
Connection ~ 4850 4950
Wire Wire Line
	4850 4350 4850 4950
Wire Wire Line
	4850 4350 4750 4350
Connection ~ 5600 4950
Wire Wire Line
	5600 4350 5600 4950
Wire Wire Line
	5600 4350 5500 4350
Wire Wire Line
	5950 3700 4300 3700
$EndSCHEMATC
