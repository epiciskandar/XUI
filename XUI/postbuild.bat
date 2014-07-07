@echo off

@set outputdir=../output

del /f/q "%outputdir%/Lib_%1/include" > nul
@mkdir "%outputdir%/Lib_%1/include" > nul
xcopy /f/c/y "*.h*" "%outputdir%/Lib_%1/include" > nul
@mkdir "%outputdir%/Lib_%1/include/WTL" > nul
xcopy /f/c/y "./WTL/*.h" "%outputdir%/Lib_%1/include/WTL/" > nul

del /f/q "%outputdir%/Lib_%1/dll" > nul
@mkdir "%outputdir%/Lib_%1/dll" > nul
xcopy /f/c/y "%outputdir%/%1/*.dll" "%outputdir%/Lib_%1/dll" > nul
xcopy /f/c/y "%outputdir%/%1/*.lib" "%outputdir%/Lib_%1/dll" > nul

if %1==Release goto release
xcopy /f/c/y "../VisualLeakDetector/bin/win32/*" "%outputdir%/%1" > nul
goto gend

:release
goto gend

:gend
exit 0