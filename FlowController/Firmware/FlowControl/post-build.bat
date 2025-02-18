@echo off
SETLOCAL=ENABLEDELAYEDEXPANSION
echo %1
copy %1\Debug\Exe\FlowControl.txt  %1\..\Binaries\FlowControl.txt