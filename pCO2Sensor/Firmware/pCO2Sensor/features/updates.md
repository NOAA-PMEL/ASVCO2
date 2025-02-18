# List of feature additions

## v1.9.4
### Issue 141
* Fixing Data headers from:
  * ***WAS:*** State,TS,CO2(ppm),Li_Temp(C),Li_Pres(kPa),Li_Raw,Li_Ref,RH(%),RH_T(C),O2(%),xCO2(dry),xCO2_(wet), co2kzero, co2kspan, co2kspan2, errors
  * ***NOW IS:*** State,TS,CO2(ppm),Li_Temp(C),Li_Pres(kPa),Li_Raw,Li_Ref,RH(%),RH_T(C),O2(%),SW_xCO2_(dry),Atm_xCO2(dry) , co2kzero, co2kspan, co2kspan2, errors


## v1.9.5
### Issue 145
* Increase timeout time on Command port
  * Timeout is now 60 seconds (was previously 10 seconds)

## v1.9.7
### Issue 148
* Remove ssconc as settable variable
  * ssconc is another variable id for span2, but is not settable
  * User settable has been removed for ssconc. 
  * span2 still works.

## v1.9.9
### Issue 149
* Spandiff error in firmware v1.9.1
  * Ran through multiple tests with no spandiff (spandiff=0.1, actual diff=0.0035)
    * No error found
  * Manually set atmospheric to 101.7kPa
    * Still no error
  * May have been fixed in v1.9.3 although no code showing changes at that point

## v1.9.11
### Issue 155
* Change span concentration to 300ppm default
  * default was 500ppm
  * now is 300ppm

## v1.9.13
### Issue 166
* Adding Flow Control pass-through mode.
  * in ***TEST*** mode, use the command ```flowpass``` to enter flow passthrough mode.
  * pCO2 now acts like a bidirectional passthrough for the flow control module.

## v1.9.14
### Issue 166
* Improving flow control pass-through.
  * ASCII ACK(0x06)/NAK(0x15) is interpreted and written as a string
  * Removal of characters under (0x20) except for ACK, NAK, CR and LF
  * More consistent pass-through experience.


## v1.10.0
Updates completed during M.C. contract through 7/22

[v1.9 Milestones to reach 1.10](https://github.com/NOAA-PMEL/EDD-pCO2Sensor/milestone/6)

* Main Control board Improved error handling conditions
* Flow control board improved error handling conditions
* Self-recovery and error logging of reset types
* SData improved sampling to capture 60 samples (also with normal logging)
* SPON flow time is now it's own setting
* Span, Span2 coefficient resilience
* Valve pulse time now settable from main controller
* Valve current setting now settable from main controller
* idata and sdata headings changed.  
* span gas default is now 300ppm
* ssconc is now read-only
* command timeout length increased from 10sec to 60sec
* tsq now recovers write after completion/reset
