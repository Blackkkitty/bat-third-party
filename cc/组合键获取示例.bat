@echo off

echo.1.查看帮助
echo.命令 cc ?
cc ?
pause

echo.2.等待获取按键组合：q 或 w+e 或 ctrl+s 或 ctrl+alt+d 或 z+x+c
echo.若返回值为1则获取q，返回值为2则获取到w+e，
echo.若返回值为3则获取到ctrl+s，返回值为4则获取到ctrl+alt+d
echo.若返回值为获取到z+x+c
echo.命令 cc K "81 87+69 17+83 17+18+68 90+88+67"
cc K "81 87+69 17+83 17+18+68 90+88+67"
echo.返回值:%errorlevel%
echo.注意，等待获取的按键组合中，不能有一个组合是另一个组合子集的情况
echo.如不能"81 81+69" 不能"81+87 81+87+69"
echo.但是可以"81+87 81+69"
pause
cc 100
echo.3.获取任意键（包括鼠标键）对应键值，返回值为对应键值
echo.命令 cc K _
:loop
cc K _
echo.返回值:%errorlevel%
goto:loop

pause
exit