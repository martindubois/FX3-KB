@echo off

rem Author   KMS - Martin Dubois, ing.
rem Product  FX3-KB
rem File     Export_Hardware.cmd

echo Executing  Export_Hardware.cmd %1 ...

rem ===== Configuration =====================================================

set EXPORT_FOLDER=K:\Export\FX3-KB

rem ===== Initialisation ====================================================

set DST="%EXPORT_FOLDER%\%1_Hardware"

set KMS_COPY_EXE="C:\Software\KmsTools\KmsCopy.exe"

rem ===== Verification ======================================================

if ""=="%1" (
    echo USER ERROR  Invalid command line
    echo Usage  .\Export_Hardware.cmd {Maj.Min.Build}
    pause
    exit /B 1
)

if exist %DST% (
    echo FATAL ERROR  %DST% exist
    pause
    exit /B 2
)

if not exist "%EXPORT_FOLDER%" (
    echo FATAL ERROR  "%EXPORT_FOLDER%"  does not exist
    echo Create the export folder on the server
    pause
    exit /B 3
)

if not exist %KMS_COPY_EXE% (
    echo FATAL ERROR  %KMS_COPY_EXE%  does not exist
    echo Install KmsTools 2.4.2 or higher
    pause /B 4
)

rem ===== Execution =========================================================

%KMS_COPY_EXE% . %DST% Export_Hardware.cmd.txt
if ERRORLEVEL 1 (
    echo ERROR  %KMS_COPY_EXE% . %DST% Export_Hardware.cmd.txt  failed - %ERRORLEVEL%
    pause
    exit /B 5
)

rem ===== End ===============================================================

echo OK
