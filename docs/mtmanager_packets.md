# XSens Golden Packets

Recorded with xsens MT manager and decoder.
Documented as reference material.
Used in unit tests.

Most of these examples describe the actual payloads.

Typical Preamble and device ID is 0xFA 0xFF
Followed by message ID. MTData2 is 0x36
Then payload.
Then CRC

## General Info

Product Code: MTi-300-2A5G4
Firmware: 1.8.2 build 37 rev 70964
Hardware: 2.0.0
Location ID: 0
Filter Setting: General
HardwareID: 15004782
Device ID: 037003F8
Self-test 000001FF
Test-Calibration Date: 21/05/2015

# Packet Breakdown

This is a packet which has most fields reported.

Size: 139 - 0x8B
Message ID (MID): 36
Items: 11

## Raw Data

```
10 20 02 A6 55 10 60 04 00 57 47 2E 20 10 10 3F 7F 7D C4 BC 10 10 7D 3B A1 57 70 BD 7E C6 56 40 20 0C BD A2 1A F8 BE 2A 8E 96 41 1D 27 A2 40 10 0C B9 4F C8 00 B9 DA 24 00 3C C9 28 80 40 30 0C 3C 02 C8 9C 3C 35 F6 C8 3C DB 0C 00 80 20 0C BB B1 7D 81 BB 96 32 00 3C 01 E8 41 80 30 10 3F 80 00 00 B6 E3 30 01 B6 C0 40 00 37 26 48 00 C0 20 0C BE 99 9C 24 3F B6 1B 56 3F 16 6A EB 30 10 04 00 01 86 DE E0 20 04 00 40 00 03
```

## Decoded Info

```
{(PacketCounter, 2 bytes, 42581), (SampleTimeFine, 4 bytes, 5719854), (Quaternion|Float|ENU, 16 bytes, (q0:   0.99801278, q1:  -0.00879299, q2:   0.00492375, q3:  -0.06220087)), (Acceleration|Float, 12 bytes, (accX:  -0.07915300, accY:  -0.16655955, accZ:   9.82217598)), (DeltaV|Float, 12 bytes, (x:  -0.00019816, y:  -0.00041607, z:   0.02455544)), (FreeAcceleration|Float, 12 bytes, (freeAccX:   0.00798240, freeAccY:   0.01110620, freeAccZ:   0.02673912)), (RateOfTurn|Float, 12 bytes, (gyrX:  -0.00541657, gyrY:  -0.00458360, gyrZ:   0.00792891)), (DeltaQ|Float, 16 bytes, (q0:   1.00000000, q1:  -0.00000677, q2:  -0.00000573, q3:   0.00000991)), (MagneticField|Float, 12 bytes, (magX:  -0.30001938, magY:   1.42270923, magZ:   0.58756894)), (BaroPressure, 4 bytes, Pressure: 100062), (StatusWord, 4 bytes, 00000000010000000000000000000011)}
```

### Item 0: Packet Counter - 10 20

Uint16 - 0x02
42581 - A6 55

### Item 1: SampleTimeFine - 10 60

Uint32 - 0x04
5719854 - 00 57 47 2E

### Item 2: Quaternion - 20 10

Float32 x4 - 0x10
q0 - 3F 7F 7D C4 - 0.9980128
q1 - BC 10 10 7D - -0.0087930
q2 - 3B A1 57 D4 - 0.0049238
q3 - 41 1D 27 A2 - 9.8221760

### Item 3: Acceleration - 40 20

Float32 x 3 - 0x0C
accX - -0.0791530 - BD A2 1A F8
accY - -0.1665595 - BE 2A 8E 93
accZ - 9.8221760 - 41 1D 27 A2

### Item 4: DeltaV - 40 10

Float32 x 3 - 0x0C
x - -0.0001982 - B9 4F D3 E8
y - -0.0004161 - B9 DA 27 FF
z - 0.0245554 - 3C C9 28 68

### Item 5: FreeAcceleration - 40 30

Float32 x 3 - 0x0C
freeAccX - 0.0079824 - 3C 02 C8 9D
freeAccY - 0.0111062 - 3C 35 F6 C7
freeAccZ - 0.0267391 - 3C DB 0B F5

### Item 6: RateOfTurn - 80 20

Float32 x 3 - 0x0C
gyrX - -0.0054166 - BB B1 7D BC
gyrY - -0.0045836 - BB 96 32 06
gyrZ - 0.0079289 - 3C 01 E8 38

### Item 7: DeltaQ - 80 30

Float32 x 4 - 0x10
q0 - 1.0000000 - 3F 80 00 00
q1 - -0.0000068 - B6 E4 2B 8E
q2 - -0.0000057 - B6 BF 42 A1
q3 - 0.0000099 - 37 26 18 2D

### Item 8: MagneticField - C0 20

Float32 x 3 - 0x0C
magX - -0.3000194 - BE 99 9C 25
magY - 1.4227092 - 3F B6 1B 56
magZ - 0.5875689 - 3F 16 6A EA

### Item 9: Barometer - 30 10

Uint32 - 0x04
100062 - 00 01 86 DE

### Item 10: StatusWord - E0 20

BitField32 - 0x04
no decoded content - 00 40 00 03

## Temp Field - 08 10

Wasn't in this longform packet.
Float32 - 0x04
37.6250000 - 42 16 80 00

# Packet Example 2

Has Mag, no temp

Size: 132 bytes - 0x84
10 items
Message ID (MID): 36

## Raw Data

```
10 20 02 A6 51 10 60 04 00 57 46 CA 20 10 10 3F 7F 7D AF BC 10 17 8D 3B A1 5D 51 BD 7E DC 78 40 20 0C BD 9A 97 A6 BE 26 F9 BF 41 1C B2 E4 40 10 0C B9 46 44 00 B9 D5 9C 00 3C C8 93 10 40 30 0C 3C 3F EE 14 3C 5F 69 B0 BA F2 80 00 80 20 0C BB 70 6E 03 BB C2 3D 00 BB D4 99 03 80 30 10 3F 80 00 00 B6 99 E0 01 B6 F8 A0 00 B7 08 10 00 C0 20 0C BE 91 DC FF 3F B6 6C 36 3F 18 71 68 E0 20 04 00 40 00 03
```

## Decoded Info

```
{(PacketCounter, 2 bytes, 42577), (SampleTimeFine, 4 bytes, 5719754), (Quaternion|Float|ENU, 16 bytes, (q0:   0.99801153, q1:  -0.00879468, q2:   0.00492445, q3:  -0.06222197)), (Acceleration|Float, 12 bytes, (accX:  -0.07548456, accY:  -0.16306208, accZ:   9.79367447)), (DeltaV|Float, 12 bytes, (x:  -0.00018908, y:  -0.00040743, z:   0.02448419)), (FreeAcceleration|Float, 12 bytes, (freeAccX:   0.01171448, freeAccY:   0.01363604, freeAccZ:  -0.00185013)), (RateOfTurn|Float, 12 bytes, (gyrX:  -0.00366867, gyrY:  -0.00592768, gyrZ:  -0.00648797)), (DeltaQ|Float, 16 bytes, (q0:   1.00000000, q1:  -0.00000459, q2:  -0.00000741, q3:  -0.00000811)), (MagneticField|Float, 12 bytes, (magX:  -0.28488919, magY:   1.42517734, magZ:   0.59548044)), (StatusWord, 4 bytes, 00000000010000000000000000000011)}
```

# Packet Example 3

No mag, no temp

Size 117 bytes
9 Items
Message ID (MID): 36

## Raw Data

```
10 20 02 8D 90 10 60 04 00 54 DB F1 20 10 10 3F 7F 89 11 BC 11 1D FA 3B A0 CE F0 BD 73 25 73 40 20 0C BD DC F9 D0 BE 3C 86 19 41 1D 0B 47 40 10 0C B9 8D B0 00 B9 F1 48 00 3C C9 04 30 40 30 0C BC B9 89 2E BB 09 8C 00 3C A7 08 00 80 20 0C BA 63 BC 01 BC 04 D6 3F BB 6D E3 FE 80 30 10 3F 80 00 01 B5 91 C0 00 B7 2A 08 00 B6 98 40 00 E0 20 04 00 40 00 03
```

## Decoded Info

```
{(PacketCounter, 2 bytes, 36240), (SampleTimeFine, 4 bytes, 5561329), (Quaternion|Float|ENU, 16 bytes, (q0:   0.99818522, q1:  -0.00885724, q2:   0.00490748, q3:  -0.05936189)), (Acceleration|Float, 12 bytes, (accX:  -0.10789835, accY:  -0.18410529, accZ:   9.81525326)), (DeltaV|Float, 12 bytes, (x:  -0.00027025, y:  -0.00046021, z:   0.02453813)), (FreeAcceleration|Float, 12 bytes, (freeAccX:  -0.02264842, freeAccY:  -0.00209880, freeAccZ:   0.02038956)), (RateOfTurn|Float, 12 bytes, (gyrX:  -0.00086874, gyrY:  -0.00810772, gyrZ:  -0.00362992)), (DeltaQ|Float, 16 bytes, (q0:   1.00000012, q1:  -0.00000109, q2:  -0.00001013, q3:  -0.00000454)), (StatusWord, 4 bytes, 00000000010000000000000000000011)}
```

# Packet Example 4

Temp, Mag, and all other IMU data

Size 147 - 0x92
12 Items
Message ID (MID): 36

## Raw Data

```
10 20 02 91 8D 10 60 04 01 36 3F A6 20 10 10 3F 35 E0 42 3F 31 CD 15 BD 9F 49 DB BD A9 38 CF 40 20 0C BD 63 5A 90 41 1D 08 D4 3E 5F AA 50 40 10 0C B9 11 68 00 3C C9 01 00 3A 0F 7D 00 40 30 0C BC 3B 29 80 3C 35 FC 00 3C A4 6E 00 80 20 0C 3C AE A2 41 BB 56 D8 00 BA D5 AC 01 80 30 10 3F 80 00 00 37 DF 88 01 B6 89 80 00 B6 08 C0 00 C0 20 0C BE FB FB F2 3F 33 C4 85 BF A0 A2 C1 08 10 04 42 16 80 00 30 10 04 00 01 86 E1 E0 20 04 00 40 00 03
```

## Decoded Info

```
{(PacketCounter, 2 bytes, 37261), (SampleTimeFine, 4 bytes, 20332454), (Quaternion|Float|ENU, 16 bytes, (q0:   0.71045315, q1:   0.69453555, q2:  -0.07777759, q3:  -0.08262789)), (Acceleration|Float, 12 bytes, (accX:  -0.05550629, accY:   9.81465530, accZ:   0.21842313)), (DeltaV|Float, 12 bytes, (x:  -0.00013867, y:   0.02453661, z:   0.00054736)), (FreeAcceleration|Float, 12 bytes, (freeAccX:  -0.01142347, freeAccY:   0.01110744, freeAccZ:   0.02007198)), (RateOfTurn|Float, 12 bytes, (gyrX:   0.02131760, gyrY:  -0.00327826, gyrZ:  -0.00163019)), (DeltaQ|Float, 16 bytes, (q0:   1.00000000, q1:   0.00002665, q2:  -0.00000410, q3:  -0.00000204)), (MagneticField|Float, 12 bytes, (magX:  -0.49215657, magY:   0.70221740, magZ:  -1.25496686)), (Temperature|Float, 4 bytes, Temp:  37.62500000), (BaroPressure, 4 bytes, Pressure: 100065), (StatusWord, 4 bytes, 00000000010000000000000000000011)}
```

# Packet Example 5

Measured during high-intensity shake
Expect that `FilterValid` isn't high
Expect that various sensors are reporting saturation in the status bitfield.
Message ID (MID): 36

## Raw Data

```
10 20 02 FB 85 10 60 04 01 A4 98 DE 20 10 10 3F 2A 14 63 BE D7 EF A7 3C DE E5 08 3F 1D CE C9 40 20 0C C1 F2 46 C3 C1 EC E0 76 C2 8F 85 3F 40 10 0C BD 93 2C CC BD 92 0A 16 BE 3A 6E EC 40 30 0C 42 51 94 64 C2 7B 5A 5A C1 CC C0 AE 80 20 0C 40 85 4D 6D C1 25 55 9E C0 90 8E 1F 80 30 10 3F 7F F8 98 3B AA 9E E5 BC 53 9E C0 BB B9 06 0C C0 20 0C 3E DC 74 39 BE 75 2B 48 3F AF 9A 3F 30 10 04 00 01 86 DE E0 20 04 00 48 14 01
```

## Decoded Info

```
{(PacketCounter, 2 bytes, 64389), (SampleTimeFine, 4 bytes, 27564254), (Quaternion|Float|ENU, 16 bytes, (q0: 0.66437358, q1: -0.42175028, q2: 0.02720882, q3: 0.61643654)), (Acceleration|Float, 12 bytes, (accX: -30.28455162, accY: -29.60960007, accZ: -71.76024628)), (DeltaV|Float, 12 bytes, (x: -0.07186279, y: -0.07130830, z: -0.18206376)), (FreeAcceleration|Float, 12 bytes, (freeAccX: 52.39491272, freeAccY: -62.83823395, freeAccZ: -25.59408188)), (RateOfTurn|Float, 12 bytes, (gyrX: 4.16570139, gyrY: -10.33340263, gyrZ: -4.51734877)), (DeltaQ|Float, 16 bytes, (q0: 0.99988699, q1: 0.00520693, q2: -0.01291627, q3: -0.00564647)), (MagneticField|Float, 12 bytes, (magX: 0.43057421, magY: -0.23942292, magZ: 1.37189472)), (BaroPressure, 4 bytes, Pressure: 100062), (StatusWord, 4 bytes, 00000000010010000001010000000001)}
```



# Packet Example 6

Only SampleTime, PacketCounter, Orientation (quaternion) and StatusWord are enabled.

Length 38 - 0x24
4 Items
Message ID (MID): 36

## Raw Data

```
10 20 02 46 82 10 60 04 01 C4 FC 3E 20 10 10 3F 71 CE 6C BE A5 6B CF 3C 61 3B D8 BD 69 1D 25 E0 20 04 00 40 00 03
```

## Decoded Info

```
{(PacketCounter, 2 bytes, 18050), (SampleTimeFine, 4 bytes, 29686846), (Quaternion|Float|ENU, 16 bytes, (q0:   0.94455600, q1:  -0.32308814, q2:   0.01374718, q3:  -0.05691256)), (StatusWord, 4 bytes, 00000000010000000000000000000011)}
```

# Configuration Example

Bytes _written_ to IMU to configure behaviours

```
FA FF
MID
SIZE
PAYLOAD
CRC
```

## GoToConfig - 0x30

00
D1

## SetStringOutputType - 0x8E

02
00 00
71

## SetOutputConfiguration - 0xC0

30
10 20 FF FF 10 60 FF FF 20 10 01 90 40 20 01 90 40 10 01 90 40 30 01 90 80 20 01 90 80 30 01 90 C0 20 00 64 08 10 00 0A 30 10 00 32 E0 20 FF FF
99

```
{(PacketCounter, Freq: 65535), (SampleTimeFine, Freq: 65535), (Quaternion|Float|ENU, Freq: 400), (Acceleration|Float, Freq: 400), (DeltaV|Float, Freq: 400), (FreeAcceleration|Float, Freq: 400), (RateOfTurn|Float, Freq: 400), (DeltaQ|Float, Freq: 400), (MagneticField|Float, Freq: 100), (Temperature|Float, Freq: 10), (BaroPressure, Freq: 50), (StatusWord, Freq: 65535)}
```

## InitMT - 0x02

00
FF

## RegConfiguration - 0x0C

00
F5

## ReqFWRev - 0x12

00
EF

## ReqAvailableFilterProfiles - 0x62

00
9F

## RegEMTS - 0x90

02
00 FF
70

Unknown data format

## RegAvailableFilterProfiles - 0x62

00
9F

## GoToMeasurement - 0x10

00

F1

## GoToConfig - 0x30

00
D1

## InitMT - 0x02

00
FF

## RegConfiguration - 0x0C

00
F5

## ReqFWRef - 0x12

00
EF

## ReqAvailableFilterProfiles - 0x62

00
9F

## RegEMTS - 0x90

02
00 FF
70

## RegAvailableFilterProfiles - 0x62

00
9F

## GoToMeasurement - 0x10

00
F1

# Configuration Response

Bytes recieved during the config write process

## GoToConfigAck - 0x31

00
D0

## StringOutputType - 0x8F

00
72

```
<no data (possible: StringOutputType, 2 bytes)>
```

## OutputConfiguration - 0xC1

08
10 20 FF FF 10 60 FF FF
9C

```
{(PacketCounter, Freq: 65535), (SampleTimeFine, Freq: 65535)}
```

## InitMTResults - 0x03

04
03 70 03 F8
8C

```
DeviceID: 037003F8
```

## Configuration - 0x0D

76
03 70 03 F8 04 80 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 01 03 70 03 F8 00 00 00 00 00 00 00 01 00 27 01 08 02 49 05 01
9B

```
(DeviceID: 037003F8, Sample Period: 1152, OutputSkipFactor: 0, SyncInMode: 0, SyncInSkipFactor: 0, SyncInOffset: 0, (Year: 0, Month: 0, Day: 0), (Hour: 0, Min: 0, Sec: 0, Ms: 0), reserved: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00, reserved: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00, numDevices: 1, DeviceID: 037003F8, MTDataLength: 0, OutputMode: 0, OutputSettings: 1, reserved: 00 27 01 08 02 49 05 01)
```

## FirmwareRev - 0x13

0B
01 08 02 00 00 00 25 00 01 15 34
69

```
(major: 1, minor: 8, revision: 2, buildnr: 37, svnrev: 70964)
```

## AvailableFilterProfiles - 0x63

6E
27 0F 67 65 6E 65 72 61 6C 20 20 20 20 20 20 20 20 20 20 20 20 20 28 0F 68 69 67 68 5F 6D 61 67 5F 64 65 70 20 20 20 20 20 20 20 20 29 0F 64 79 6E 61 6D 69 63 20 20 20 20 20 20 20 20 20 20 20 20 20 2A 0F 6C 6F 77 5F 6D 61 67 5F 64 65 70 20 20 20 20 20 20 20 20 20 2B 0F 76 72 75 5F 67 65 6E 65 72 61 6C 20 20 20 20 20 20 20 20 20
F1

```
{(FilterProfileType: 39, Version: 15, Label: general), (FilterProfileType: 40, Version: 15, Label: high_mag_dep), (FilterProfileType: 41, Version: 15, Label: dynamic), (FilterProfileType: 42, Version: 15, Label: low_mag_dep), (FilterProfileType: 43, Version: 15, Label: vru_general)}
```

## EMTSData 0x91

05 28
96 50 5E 55 1E 23 37 7F 4F 5A 71 4D 11 1B 13 7D 1E 6C 76 5C 0F 09 5C 65 3C 4D 68 6D 5A 34 A5 60 76 25 6A 3B 84 31 32 58 38 4C 31 76 7A 5F 3A 39 71 33 3C 67 69 55 51 36 2D 49 2E 43 7E 3F 4D 50 E5 61 76 3E 36 7E 62 31 0B 68 D3 BB 51 52 8D A6 2E 54 44 28 34 2D 31 5D 48 25 5E 3E 41 7A 44 55 7A 50 5B 52 79 52 62 27 6B 21 56 6B 6E 7C 68 64 42 2A 77 4A 52 69 3C 35 35 3E 7E 56 29 4E 6D 65 6C 54 2F 44 73 55 7A 6E 29 35 6A 44 39 58 3D 33 5B 48 5B 61 40 2A 29 63 58 72 2E 4B 55 35 5A 61 2A 23 29 31 3E 4A 78 2F 59 48 5D 46 29 27 41 24 63 6F 3E 66 70 5D 67 60 3A 2A 3C 3A 2B 45 52 28 55 4D 51 61 4E 55 52 77 5B 67 38 6B 54 8D 7E 6A 4B 7E CA 56 3F 6E 53 2E 56 53 77 5E 52 7C 6A 41 60 23 5A 26 3A 2A 4C 56 7C 2F 29 7C 45 3C 4D 68 6F 5B 55 27 00 76 30 6F 39 7B 31 32 58 38 4C 31 76 7A 5F 3A 39 71 33 3C 67 68 55 51 36 2D 49 2F 43 7E 27 4D 50 65 65 76 C1 37 7E 62 31 2B 78 2C E5 95 51 72 59 2E 54 44 28 34 2D 31 5D 48 25 5E 3E 41 7A 44 56 78 51 5B 52 79 52 62 2D 6B 21 56 6F 6A 7D 68 64 42 2A 77 40 52 69 3C 35 35 3E 7E 56 29 4E 6D 65 6C 54 2F 44 73 55 7A 6E 29 35 6A 44 39 58 3D 33 5B 48 5B 61 40 2A 29 63 58 72 2E 4B 55 35 5A 61 2A 23 29 31 3E 4A 78 2F 59 48 5D 46 29 27 41 24 63 6F 3E 66 70 5D 67 60 3A 2A 3C 3A 2B 45 52 28 55 4D 51 61 4E 55 52 77 5B 67 38 6B 32 6B 7E 6A 49 7E 5A 57 3F 6E 53 2E 56 53 77 5E 52 7C 6A 41 60 23 5A 26 3A 2A 4C 56 7C 2F 29 7C 45 3C 4D 68 6F 5B 31 27 27 76 30 6F 39 7B 31 32 58 38 4C 31 76 7A 5F 3A 39 71 33 3C 67 68 55 51 36 2D 49 2F 43 7E 3F 4D 50 65 65 76 3E 36 7E 62 31 2B 78 2C 44 31 42 72 59 2E 54 44 28 34 2D 31 5D 48 25 5E 3E 41 7A 44 55 7A 50 5B 52 79 52 62 27 6B 21 56 6B 6E 7C 68 64 42 2A 77 4A 52 69 3C 35 35 3E 7E 56 29 4E 6D 65 6C 54 2F 44 73 55 7A 6E 29 35 6A 44 39 58 3D 33 5B 48 5B 61 40 2A 29 63 58 72 2E 4B 55 35 5A 61 B0 8F 3F 01 BF 11 0F 58 18 08 3C 85 FA 0C 24 77 02 2F 3E 66 70 5D 67 60 7E 6A 3C 3A AB 7A 52 28 55 4D 51 61 4E 55 52 77 5B 67 38 6B B2 54 7E 6A 49 7E 5A 57 3F 6E 53 2E 56 53 77 5E 52 7C 6A 41 60 23 5A 26 3A 2A 4C 56 FC 10 29 7C 45 3C 4D 68 6F 5B 31 27 27 76 30 6F 39 7B 60 9A A7 7E 31 1A 76 3D 72 2C 39 36 71 13 99 2E F0 4C C9 6B 9A 1F 43 39 BC 4D D0 5A 6F 83 3C 0A 94 D3 2C 17 FF 6E 95 8A 0B 70 D9 11 79 6D 1C 0F AA 5A 9A 72 B4 63 AA 7D 23 BF 2A 45 14 A4 2D 46 FA 92 11 50 F9 C5 26 52 82 0F D6 79 B7 88 35 6D B2 1F 02 89 B0 21 91 92 EA 31 BF D7 56 2E C4 4C 4B 0A A1 6A F2 31 94 7A 3A 28 E0 18 BB D0 5E 05 DA B0 22 1D FA 9A 75 10 D9 96 FA 94 0D DA D0 80 EF 60 0A 19 3C A2 E4 96 7A 03 7B A3 47 3B FB 4F F3 13 9D 01 75 B3 CD 10 C9 13 CA 6E DF 5E 31 70 59 ED FC E4 FA 24 1D 0C DA C7 23 72 F1 70 AB 04 8F 90 56 ED 92 BF FA 69 23 23 40 5C CB 6B CE 81 2A 4C 56 7C 2F 29 7C 45 3C 4D 68 6F 5B 31 27 27 76 30 6F 39 7B 31 32 58 F4 12 1D 41 9A 98 E0 0E 8E 9D CD DF F4 4F B3 0E 11 4E D3 F4 03 F9 8F E7 10 59 64 86 A1 19 BA 06 37 BB DD 72 43 F6 C4 67 40 C1 FD 16 8C 44 0C 62 44 EE 5B 01 C2 7D 45 6A ED 82 58 6D 79 52 62 27 6B 21 56 6B 6E 7C 68 64 42 2A 77 4A 52 69 A0 77 35 7E 62 12 46 5C 6E 5E 03 46 2C 7F 1C 47 79 55 23 E2 49 78 33 8F 1E 0F 51 9F 78 5D 40 2A 29 63 58 72 2E 4B 55 35 5A 61 2A 23 61 72 3E 4A 30 6C 59 48 15 05 29 27 41 24 63 6F 3E 66 70 5D 67 60 0E AA 8B 01 1F C5 E5 13 61 CD E6 5A 4E 55 52 77 5B 67 38 6B 32 6B 7E 6A 49 7E 5A 57 7F 68 05 3D 30 5B 33 48 50 26 A0 15 60 23 5A 26 3A 2A 4C 56 7C 2F 29 7C 45 3C 4D 68 BC D4 CE 61 D3 8B CF 29 F6 7A CE 74 80 70 33 0E 51 90 09 87 8D 27 57 01 9A 5A AD 6D 1A BB 36 10 B1 2C 03 F0 00 B8 FE CB CF 80 CD D9 B0 10 07 13 9F F4 80 31 60 71 97 07 F3 26 F2 72 06 38 91 60 82 84 54 F9 B3 00 8D 65 C7 76 CD DD F4 AF 84 E9 62 BA B3 D7 2F A2 C2 CD 6D 64 AD 85 DE 39 A1 C4 5C 18 11 54 74 41 7E 15 A3 3B 4E 41 84 A4 5D 51 E2 32 38 06 05 E7 00 60 65 40 2A 29 63 58 72 2E 4B 55 FB 5F 0D 22 23 21 5E 2C C9 43 40 4B CB 66 29 3B A4 7A 24 63 6F 3E 66 70 5D 67 60 3A 2A 3C 3A 2B 45 52 28 55 4D 51 61 4E 55 52 77 5B 67 38 6B 32 6B 7E 6A 49 7E 1A 57 3F 6E 53 2E 56 53 77 5E 52 7C 6A 41 60 23 5A 26 3A 2A 4C 56 7C 2F 29 7C 45 3C 4D 68 6F 5B 31 27 27 76 30 6F 39 7B 31 32 58 38 4D
F0

```
<unknown format>
```

## AvailableFilterProfiles - 0x63

6E
27 0F 67 65 6E 65 72 61 6C 20 20 20 20 20 20 20 20 20 20 20 20 20 28 0F 68 69 67 68 5F 6D 61 67 5F 64 65 70 20 20 20 20 20 20 20 20 29 0F 64 79 6E 61 6D 69 63 20 20 20 20 20 20 20 20 20 20 20 20 20 2A 0F 6C 6F 77 5F 6D 61 67 5F 64 65 70 20 20 20 20 20 20 20 20 20 2B 0F 76 72 75 5F 67 65 6E 65 72 61 6C 20 20 20 20 20 20 20 20 20
D0

```
{(FilterProfileType: 39, Version: 15, Label: general), (FilterProfileType: 40, Version: 15, Label: high_mag_dep), (FilterProfileType: 41, Version: 15, Label: dynamic), (FilterProfileType: 42, Version: 15, Label: low_mag_dep), (FilterProfileType: 43, Version: 15, Label: vru_general)}
```

## GotoMeasurementAck - 0x11

00
8C

## GotoConfigAck

This is because the configuration seems to occur twice?
