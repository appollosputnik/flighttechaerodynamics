Setlocal EnableDelayedExpansion

call "%~dp0env.bat" %1 %2 %3

set "BIN_DIR=win%ARCH%\%VCVER%\bind"
if ["%CASDEB%"] == [""] (
  set "BIN_DIR=win%ARCH%\%VCVER%\bin"
)

if not exist "%~dp0%BIN_DIR%\FlightTechAerodynamics.exe" goto ERR_EXE

echo Starting FlightTechAerodynamics .....
"%~dp0%BIN_DIR%\FlightTechAerodynamics.exe"

goto END

:ERR_EXE
echo Executable %~dp0%BIN_DIR%\FlightTechAerodynamics.exe not found.
echo Probably you didn't compile the application.
pause
goto END

:END