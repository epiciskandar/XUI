@echo off 

@echo ================	Build	=================
"%VS11PATH%\devenv.com" .\XUI.sln /rebuild "Release|Win32"
if ERRORLEVEL 1 exit /b 1
@echo .
@echo .
@echo ================	End~~	=================
@exit /b 0
