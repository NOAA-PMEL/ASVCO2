# pCO2 Operational States
## Overview
The following are the operation flow control states for the MApCO2 system.


## Operation State

| State | Description | Cycle | Command |Pump | V1 | V2 | V3 | V4 | V5 | V6 |
|-------|-------------|------------|--------------|-----|----|----|----|----|----|----|
| 1 | Zero Pump ON | Zero | ZPON | ON | CB | CB | CB | CB | CA | CB |
| 2 | Zero Pump OFF | Zero | ZPOFF | OFF | CB | CB | CB | CB | CA | CB |
| 3 | Zero Vent | Zero | ZPVENT | OFF | CB | CB | CB | CA | CA | CB |
| 4 | Zero Post | Zero | ZPPCAL | OFF | CB | CB | CB | CB | CA | CB |
| 5 | Span Flow ON | SPAN | SPON | OFF | CA | CB | CB | CA | CB | CB |
| 6 | Span Flow OFF | SPAN | SPOFF | OFF | CA | CB | CB | CA | CA | CB |
| 7 | Span Vent | SPAN | SPVENT | OFF | CA | CB | CB | CA | CA | CB |
| 8 | Span Post | SPAN | SPPCAL | OFF | CA | CB | CB | CB | CA | CB |
| 9 | Equil Pump ON | EQUIL | EPON | ON | CB | CA | CA | CB | CA | CA |
| 10 | Equil Pump OFF | EQUIL | EPOFF | OFF | CB | CA | CA | CB | CA | CA | 
| 11 | Equil Vent | EQUIL | EPVENT | OFF | CA | CB | CB | CA | CA | CA | CA |
| 12 | Equil Post | EQUIL | EPPOST | OFF | CA | CB | CB | CA | CA | CA | CA |
| 13 | Air Pump ON | AIR | APON | ON | CA | CB | CB | CA | CA | CA |
| 14 | Air Pump OFF | AIR | APOFF | OFF | CA | CB | CB | CA | CA | CA |
| 15 | Air Vent | AIR | APVENT | OFF | CA | CB | CB | CA | CA | CA | 
| 16 | Air Post | AIR | APPOST | OFF | CA | CB | CB | CA | CA | CA |
| 17 | Rest | REST | REST | OFF | CB | CB | CB | CB | CA | CB |
| 18 | Purge 1 | PURGE | PRG1 | ON | CA | CB | CA | CB | CA | CA |
| 19 | Purge 2 | PURGE | PRG2 | OFF | CB | CA | CA | CB | CB | CA |
| 20 | Purge 3 | PURGE | PRG3 | OFF | CB | CA | CB | CA | CA | CA |
| 21 | Purge 4 | PURGE | PRG4 | OFF | CB | CB | CB | CA | CA | CA |
| 22 | Purge 5 | PURGE | PRG5 | OFF | CA | CB | CB | CA | CA | CA |
| 23 | Purge 6 | PURGE | PRG6 | OFF | CB | CB | CB | CB | CA | CB |
| 24 | Purge 7 | PURGE | PRG7 | OFF | CB | CB | CB | CA | CB | CB |
| 25 | Purge 8 | PURGE | PRG8 | OFF | CA | CB | CB | CA | CA | CB |

## Deployment State
| State | Description |  State Name | Command|Pump | V1 | V2 | V3 | V4 | V5 | V6 |
|-------|-------------|-------------|--------|-----|----|----|----|----|----|----|
| 1 | Deployment Mode | Deploy | DEPLOY | Off | CB | CB | CB | CB | CA | CA |


## Experimental
| State | Description | Cycle | Command |Pump | V1 | V2 | V3 | V4 | V5 | V6 |
|-------|-------------|------------|--------------|-----|----|----|----|----|----|----|
| 1 | Pressurize | PRES | PRES | ON | CA | CB | CB | CB | CA | CB |