set PLAT="x86"
IF "%Platform%"=="x64" set PLAT="x64"
set VERSION="WIN32"
IF "%Platform%"=="x64" set VERSION="WIN64"

cl.exe /D %VERSION% /c /GS- bof_callback.c /Fobof_callback.%PLAT%.o




