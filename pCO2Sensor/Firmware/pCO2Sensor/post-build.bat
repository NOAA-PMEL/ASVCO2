@echo off
SETLOCAL=ENABLEDELAYEDEXPANSION
echo %1
copy %1\Debug\Exe\pCO2Sensor.txt  %1\..\Binaries\pCO2Sensor.txt