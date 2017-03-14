@echo off

echo. 1.查看帮助
echo. 命令：jsoner
jsoner
pause

echo.
echo. 2.解析json字符串
echo. 命令：jsoner -s [1,2,true] result_1.bat obj
jsoner -s [1,2,true] result_1.bat obj
pause

echo.
echo. 2.解析json文件
echo. 命令：jsoner -f test.json result_2.bat test
jsoner -f test.json result_2.bat test

echo.
echo.结束！
pause
exit


