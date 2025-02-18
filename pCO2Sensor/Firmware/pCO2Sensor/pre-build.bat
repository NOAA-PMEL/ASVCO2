@echo off
SETLOCAL=ENABLEDELAYEDEXPANSION



REM Create Doxygen Files
REM doxygen %0\Doxygen\Doxyfile

REM Get the GIT tag/has value
REM for /f %%c in ('git branch | grep \*') do set VAR=%%c
REM if %c%=='* master'  
REM for /f %%a in ('git describe --abbrev=0')  do set VAR=%%a
REM else 
for /f %%a in ('git describe --tags --long') do set VAR=%%a
echo %VAR%

REM Setup for value to overwrite
set find=VER_VAR
set replace=%VAR%

REM Get the date
for /F "tokens=2" %%i in ('date /t') do set mydate=%%i
set mytime=%time%
REM echo %mydate% %mytime%
set dt=%mydate% %mytime%
echo %dt%
set find2=VER_DATE
set replace2=%dt%

REM Load the template and replace the version number
del %1\src\version.h
for /f "tokens=1 delims=" %%i in (%1\version_template.h ) do (
	set string=%%i
	call set string=%%string:!find!=!replace!%%
    call set string=%%string:!find2!=!replace2!%%
	>> %1\src\version.h  (echo !string!)
)
 



echo on
 