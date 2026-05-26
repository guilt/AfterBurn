@ECHO OFF
REM Debug build + GDB launch script
REM Usage: DEBUG [theme]
REM   DEBUG            -> runs GREEN theme
REM   DEBUG TAMIL      -> runs TAMIL theme

IF NOT "%EXTDEV%"=="" GOTO :HAVEEXT
ECHO ERROR: EXTDEV environment variable not set.
GOTO :END
:HAVEEXT
SET APP=%~dp0
CD /D "%APP%"

SET DEBUG=1
CALL BUILD.BAT
SET DEBUG=
IF ERRORLEVEL 1 GOTO :END

IF NOT "%~1"=="" GOTO :SETINI
SET INI=THEME/GREEN/GREEN.INI
GOTO :RUN
:SETINI
SET INI=THEME/%~1/%~1.INI
:RUN
ECHO Starting GDB with %INI% ...
%GDB% -ex "run" --args "%APP%AB.EXE" "%APP%%INI%"
:END
