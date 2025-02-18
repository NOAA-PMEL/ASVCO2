# List of pCO2 Commands (Non-Console Command Set)

If a command is listed as **R/W**, send the command with a \<CR>\<LF> to read the value.  Add an \= to the command, plus the data to set in the format shown to write the value.


## System Setting Commands
The following system commands are also available:

| Command | Read/Write | Function |
|---|---|---|
| ? | R | Display list of commands | 
| ^C | W | (x3) Exit Logging by reset (kill process and reset system) |
| ver | R | Current software/firmware version |
| serial | **R/W** | Sensor serial number |
| system | R | Display all system settings |
| tasks | R | Display task information in scheduler |
| startup | R | Startup count |
| sreset | W | Software reset | 
| reset | W | Hardware reset |


### ? 
Displays list of PCO2 Commands available to user

### ^C
Control-C: Kills current process

### Q
Quit the console and return system to sleeep mode.

### ver 
Displays the current firmware version

### serial

Sensor serial number.  Has format:
~~~
serial= yyssss
~~~
yyssss is an 6-digit number.  Will be appended to "who" to form "pco2yyssss"
* yy is 2-digit year
* ssss is 4-digit serial number

### tasks? 
Current task queue information 

### system
Displays all the system settings

### startup 
Displays the number of system resets

### sreset
Reset the system by software (instant reset)

### reset
Reset the system by triggering watchdog (delayed).

**NOTE:** This only works if the watchdog bypass jumper has been removed.


## TELOS compliant commands
| Command | Read/Write | Function |
|---|---|---|
| cshow | **R** | Show Comm Data |
| gps | **R**  | GPS Data (always -2) |
| time | **R/W** | Sensor time |
| idata | **R** | Iridium Formatted Data |
| sdata | **R** | Sensor Raw Data |
| who | **R/W** | Serial number|

### cshow
Comm data.  Includes Com name, sensor name, sample start time, sample interval and sample period.
 Format as follows:

~~~
    COMM1
    PCO2
    samplestart = HH:MM:SS
    sampleinterval = HH:MM:SS
    sampleperiod = HH:MM:SS
    NA
    samplestart = 00:00:00
    sampleinterval = 00:00:00
    sampleperiod = 00:00:00
    NA
    samplestart = 00:00:00
    sampleinterval = 00:00:00
    sampleperiod = 00:00:00
~~~

* HH is two digit hour
* MM is two digit minute
* SS is two digit second

### gps
GPS data.  Always -2 for PCO2 sensor.
Return format as follows:
~~~
    gps= -2
~~~

### time
Time data.  Can be written or read.  
~~~
    time= yy mm dd HH MM SS
~~~
* yy is two digit year
* mm is two digit month
* dd is two digit date
* HH is two digit hour
* MM is two digit minute
* SS is two digit second

### idata
Iridium formatted data.  Has format:

/* @todo Add to this */

~~~
    %%HEADER
    DT,Mode,xxxxxxxx
~~~

### sdata
Sensor raw data.  Has format:
/* @todo Add to this */
~~~
    Header
~~~

### who
Sensor serial number.  Has format:

~~~
    who=SSSSYYNNNN
~~~

* SSSS Four char Sensor Name (pco2)
* YY Two-digit year
* NNNN Serial Number

<div style="page-break-after: always;"></div>

## PCO2 Individual Configuration Commands
| Command | Read/Write | Function |
|---|---|---|
| span | **R/W** | Span Gas Concentration (ppm) |
| spandiff | **R/W** | Span Pressure Difference (kpa) |
| interval | **R/W** | Normal Mode Cycle Interval (hours) |
| change | **R/W** | Length of Fast Mode Sampling |
| equil | **R/W** | Length of equilibrator pump (seconds) |
| warmup | **R/W** | Length of LI8X0 Warmup (seconds) |
| pumpon | **R/W** | Length of pumping during a cycle (seconds) | 
| pumpoff | **R/W** | Length of pump off during a cycle (seconds) |
| sampleco2 | **R/W** | Length of data sampling during a cycle (seconds) |
| vent | **R/W** | Length of delay between opening and closing vent (seconds) |
| heater | **R/W** | Sets Licor Heater ON or OFF |
| sample | **R/W** | Data sampling interval (Hz) |

### span 

Sets the span gas concentration (in ppm).  
~~~
span= nnn.nn
~~~
* nnn.nn is floating point value for the attached span gas

### spandiff

Sets the span pressure difference (in kPa) for performing the span calibration.  

~~~
spandiff= p.pp
~~~

* Floating point value for the pressure difference.  Entering 0 sets sensor to always perform span cal.



### sample

Sample is the data sampling interval, in Hertz (1/s)

~~~
sample= n
~~~
* n is sample interval.  Acceptable values are 1, 2, or 4  (Hz)


### interval
Sets the normal mode cycle interval (hours).
~~~
interval= hh
~~~
* hh Hours between normal mode cycles

### change
Sets the fast mode cycle interval.
~~~
change= hh mm ss
~~~
All values are combined to produce total interval offset.

* hh Hours between cycles
* mm Minutes between cycles
* ss Seconds between cycles

### equil
Sets the length of equilibration pumping (seconds).
~~~
equil= sss
~~~
* sss Seconds of equilibration pumping

### warmup
Sets the length of Licor Li8x0 warmup before first cycle in sample
~~~
warmup= sss
~~~
* sss Seconds of Licor warmup

### pumpon
Sets the length of pumping during a cycle
~~~
pumpon= sss
~~~
* sss Seconds of Pump on 

### pumpoff
Sets the length of pumping off time before a sampe during a cycle
~~~
pumpoff= sss
~~~
* sss Seconds of Pump off

### sampleco2
Sets the length of time to sample over during a cycle
~~~
sampleco2= sss
~~~
* sss Seconds of sampling during a cycle

### vent
Sets the length of delay before closing the vent.  Applies to both vent 1 and 2
~~~
vent= sss
~~~
* sss Seconds of venting during a cycle

### heater
Sets the Licor heater on or off.
~~~
heater= n
~~~
* n State of Licor heater.  0 is OFF, 1 is ON

<div style="page-break-after: always;"></div>

## Logging Commands
The following commands are used for setting the PCO2 sensor into its various logging modes.  This is a write-only function.

| Command | Function | Mode |
|---|---|---|
| log | Start logging | Fast Mode |
| fast | Start logging | Fast Mode |
| normal | Start logging | Normal Mode |
| norm | Start logging | Normal Mode |
| dep | Start deployment | Deployment Mode |
| depl | Start deployment | Deployment Mode |

### log *or* fast
Starts logging with fast mode.
~~~
log
fast
~~~

### normal *or* norm
Starts logging with normal mode.
~~~
normal
norm
~~~

### dep *or *depl*
Start deployment mode
~~~
depl
dep
~~~

### test
Set the system to TEST MODE, allowing access to test commands.
~~~
test
~~~   

<div style="page-break-after: always;"></div>

## Valve Setting commands
These commands are for testing the sensor in different valve configurations.  Test mode is entered after this command.  All commands are write-only.

| Command | Function | 
|---|---|
| apos | Air position test loop |
| epos | Equilibrator position test loop |
| rpos | Rest position test loop |
| spos | Span position test loop |
| zpos | Zero position test loop |
| nopos | Start the test loop without changing valves |
| valves | Set valves to sequence selected (NOT CURRENTLY IMPLEMENTED) |
| valve | Set inidividual vale to selected (NOT CURRENTLY IMPLEMENTED) |

### apos
Sets the valves to the Air position.  Puts system in TEST MODE.
~~~
apos
~~~

### epos
Sets the valves to the Equilibrator position.  Puts system in TEST MODE.
~~~
apos
~~~

### rpos
Sets the valves to the Rest position.  Puts system in TEST MODE.
~~~
rpos
~~~

### spos
Sets the valves to the Span position.  Puts system in TEST MODE.
~~~
spos
~~~

### zpos
Sets the valves to the Zero position.  Puts system in TEST MODE.
~~~
zpos
~~~

### nopos 
Keeps valves in current positions.  Puts system in TEST MODE.
~~~
nopos
~~~

### valves

***NOT CURRENTLY IMPLEMENTED***

Set the valves to the sequence entered.

~~~
valves= x1 x2 x3 x4 x5 x6 p
~~~

* xy is the valve number and setting.
* x can be 'a' for flow 'CA' or 'b' for 'CB' (See valve flow diagrams)
* p is the Pump setting.  0 = OFF, 1 = ON

Example:
The following would set the condition for Zero with pump off, vent closed (POST CAL) 

~~~
valves= b b b b a b 0
~~~

The following would set the condition for Air Mode, pump ON

~~~
valves= a b b a a a 1
~~~

### valve

***NOT CURRENTLY IMPLEMENTED***

Set a single valve to the setting entered

~~~
valve= n x
~~~

* n is the valve number (1 - 6)
* x is the valve setting.  'a' for flow 'CA' or 'b' for 'CB' (See valve flow diagrams)

<div style="page-break-after: always;"></div>

## Test Mode Command
This command puts the system in test mode.  Test mode supresses all scheduled runs 
until the system re-enters normal mode.  The following commands are write-only.

| Command | Function |
|---|---|
| test | Start the test loop without changing valves |

### test 
Enter Test Mode.  Allows for the remaining commands in this document to be run.
~~~
test
~~~


## Licor Commands 

| Command | Read/Write |Function | 
|---|---|---|
| licoron | R | Turn Licor ON |
| licoroff | R |Turn Licor OFF |
| licorcfg | R | Run Licor Configuration Sequence |
| lizero | R/W | Zero the Licor |
| lispan | R/W | Span the Licor (Span 1) |
| lispan2 | R/W | Span the Licor (Span 2) |
| lisample | R | Read the Licor Data |
| heaton | R/W | Turn the Licor Heater ON |
| heatoff | R/W | Turn the Licor Heater OFF |   
| lipass | W | Allow direct access to LICOR UART |

### licoron 
Turn Licor ON 
~~~
licoron
~~~

### licoroff
Turn Licor OFF 
~~~
licoroff
~~~

### licorcfg
Run licor configuration routine.
~~~
licorcfg
~~~

### lizero
Zero the Licor and return the coefficient.  Without the = returns the current coefficient without zeroing the system.
~~~
lizero= 
~~~

### lispan
Span the Licor and return the coefficient.  Withouth the = returnst he current coefficient withou spanning the system.
~~~
lispan= XXXX.XX
~~~

where 
XXXX.XX is the gas concentration of CO2 in PPM.

### lispan2
High span the Licor and return the coefficient.  Withouth the = returnst he current coefficient withou spanning the system.
~~~
lispan2= XXXX.XX
~~~

where 
XXXX.XX is the gas concentration of CO2 in PPM.

### lisample
Sample the Licor.  Returns data from Licor.
~~~
lisample
~~~

### heaton
Turn the Licor Heater ON.
~~~
heaton
~~~

### heatoff
Turn the Licor Heater OFF.
~~~
heatoff
~~~

### lipass
Allow direct access to Licor UART port.  This allows for communicating directly with the Licor.  This command turns the Licor power ON and starts the pass-through.  Send 3 consecutive Ctrl-C characters to restart the system in normal mode.

~~~
lipass
~~~

## Flow Control Commands 

| Command | Function |
|---|---|
| flowon | Turn Flow Control ON |
| flowoff | Turn Flow Control OFF |

### flowon
Turn Flow Controller Power ON.  Puts the valve block in DEPLOY with PUMP OFF.
~~~
flowon
~~~

### flowoff
Turn Flow Controller Power OFF 
~~~
flowoff
~~~

## Oxygen (O2) Sensor Commands 
| Command | Function |
|---|---|
| o2 | Read O2 Sensor |
| o2volt | Read O2 Port Voltage |
| o2int | Interval in days between O2 Calibration |
| o2time | Time of day for O2 calibration to occur |
| anaon | Analog Reference ON |
| anaoff | Analog Reference OFF |

### o2 
Read the O2 sensor and display the results.

~~~
o2
~~~

The response is in the format:

~~~
O2= yyy.yyy
~~~

* yyy.yyy is percent (%) O2

### o2volt
Read the O2 Sensor port voltage and display the results.

~~~
o2volt
~~~

The response is in the format:

~~~
o2volt= yyy.yyy
~~~

* yyy.yyy is the voltage in volts (DC).

### o2int
***NOT CURRENTLY IMPLEMENTED***

### o2time 
***NOT CURRENTLY IMPLEMENTED***

### anaon 
Turn the analog power port ON 

~~~
anaon
~~~

### anaoff
Turn the analog power port OFF
~~~
anaoff
~~~

### anapwron

***NOT CURRENTLY IMPLEMENTED***

Turn Analog Voltage Reference ON
~~~
anapwron
~~~

### anapwroff
***NOT CURRENTLY IMPLEMENTED***

Turn Analog Voltage Reference OFF
~~~
anapwroff
~~~



## Relative Humidity (RH) Sensor Commands
| Command | Function |
|---|---|
| rh | Read RH Sensor (RH + Temperature) |

### rh
Read the Relative Humidity sensor anre display the results.

~~~
rh
~~~

The response is in the format:

~~~
rh= yyy.yyy
temperature= zzz.zzzdegC
~~~

* yyy.yyy is the relative humidity in %RH
* zzz.zzz is the temperature in degrees C

## Real-Time Clock (RTC) Commands
| Command | Read/Write | Function |
|---|---|---|
| rtcon | R | Output RTC 32kHz signal |
| rtcoff | R | Supress RTC 32kHz signal |
| rtcset | R/W | Set RTC 32kHz Frequency |

### rtcon
***NOT CURRENTLY IMPLEMENTED***

### rtcoff
***NOT CURRENTLY IMPLEMENTED***

### rtcset
***NOT CURRENTLY IMPLEMENTED***


## LED Commands 
| Command | Function |
|---|---|
| grnon | Turn Green LED ON |
| grnoff | Turn Green LED OFF |
| redon | Turn Red LED ON |
| redoff | Turn Red LED OFF |

### grnon
Turn Green LED ON 
~~~
grnon
~~~

### grnoff
Turn Green LED ON 
~~~
grnoff
~~~

### redon
Turn Red LED ON 
~~~
redon
~~~

### redoff
Turn Red LED OFF
~~~
redoff
~~~






### spareon
Turn Spare Power ON 
~~~
spareon
~~~

### spareoff
Turn Spare Power OFF
~~~
spareoff
~~~








<div style="page-break-after: always;"></div>


