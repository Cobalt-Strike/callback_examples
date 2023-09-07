@echo off

:: Set your Microsoft.Net framework directory
::set DOTNETDIR="C:\Windows\Microsoft.NET\Framework\v4.0.30319"
set DOTNETDIR="C:\Program Files\Microsoft Visual Studio\2022\Professional\Msbuild\Current\Bin\Roslyn"

:: Build the executable
%DOTNETDIR%\csc.exe /t:exe /out:example_ea.exe Program.cs
