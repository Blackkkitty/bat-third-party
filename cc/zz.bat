@echo off

:1
cc K _
set k=%errorlevel%
echo.%k%
::if "%k%" NEQ "0" (echo.%k%)
goto:1

:2
cc K "81 87+69"
echo.%errorlevel%
goto:2
pause
exit


