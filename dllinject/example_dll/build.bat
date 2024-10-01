@echo off

set PLAT="x86"
IF /I "%Platform%"=="x64" set PLAT="x64"

echo Building Platform %PLAT%
cl.exe ReflectiveLoader.c example_dllinject.c /LD /link /OUT:example_dllinject.%PLAT%.dll

echo Cleanup temporary files
del *.lib *.exp *.obj
