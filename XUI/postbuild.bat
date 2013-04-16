@echo off

del /f/q "../output/Lib_%1/include" > nul
mkdir "../output/Lib_%1/include" > nul
xcopy /f/c/y "XUI/*.h*" "../output/Lib_%1/include" > nul
mkdir "../output/Lib_%1/include/WTL" > nul
echo %cd%
xcopy /f/c/y "./XUI/WTL/*.h" "../output/Lib_%1/include/WTL/" > nul

del /f/q "../output/Lib_%1/dll" > nul
mkdir "../output/Lib_%1/dll" > nul
xcopy /f/c/y "../output/%1/*.dll" "../output/Lib_%1/dll" > nul
xcopy /f/c/y "../output/%1/*.lib" "../output/Lib_%1/dll" > nul

if %1==Release goto release
xcopy /f/c/y "VisualLeakDetector/bin/win32/*" "../%1" > nul
goto gend

:release
goto gend

:gend
exit 0