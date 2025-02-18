# Change Log for pCO2 Firmware
## v1.11-29
Adding Licor CO2 Polynomial values ccva1/2/3/4 to the report.
## v1.11-27
Fixing garbage characters in output.  
## v1.11-21
Fixing mfr command missing from report & commands
## v1.11-16
Fixing issue with idata/sdata not reporting in v1.11
## v1.11-10
Escape character (0x1b) sent 3 times will now reset the system.

## v1.11-8
Bug fixes.

### Added
* Added Ctrl-Z x3 as alternate restart command for command(rs-232) only.
### Fixed
* tsq created unrecoverable message verbosity.  Fixed.
* fcurrent/fpulse not showing on first call.  Added flow control restart for that.  Needs to be in test mode.

## v1.11
v1.11 added capability to the Flow Controller interface as well as addressed logging on the command port and a few error messages.
### Added
* [Issue #220](https://github.com/NOAA-PMEL/EDD-pCO2Sensor/issues/220).  Updated pCO2 firmware with Drying/Purge functionality recently added to Flow Control firmware.  New functions for running with purge are "Take Sample with Purge" (```>tsp```)  and "Take Sample Quiet with Purge" (```>tspq```) 
* [Issue #227](https://github.com/NOAA-PMEL/EDD-pCO2Sensor/issues/227). Logging interface for RS232.  Can now set the command port log level (debug or error) using ```>log= debug``` or ```>log= error``` in console.  Debug mode provides highest verbosity logging.  Error provides only data and error messages.
  
* Flow Controller pass-through mode now allows direct communication with flow control board. ```> flowpass``` puts the pCO2 board into this mode.
*  Flow pulse and Flow current now allows direct setting of these Flow Control board parameters.  ```fpulse= XXX``` sets the valve pulse length to XXX.  ```fcurrent= X``` sets the clipping current to X (0 - 7).
*  
### Changed
*  Logging module refactored.  Removed unnecessary codes and errors.c/h files which were confusing duplicates of logging.c/h.   

### Fixed
* Errors
---
## v1.10 
v1.10 primarily added functionality for integrating with the TELOS buoy controller.
### Added
* SPON time now settable. (Allows for user define Span Gas ON time to be modified for different field conditions).
### Changed
* IData and SData commands updated to interface with TELOS buoy controller
* Sample rate updated
 
### Fixed
* TS hang issue.