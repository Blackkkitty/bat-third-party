@echo off

echo. 1.�鿴����
echo. ���jsoner
jsoner
pause

echo.
echo. 2.����json�ַ���
echo. ���jsoner -s [1,2,true] result_1.bat obj
jsoner -s [1,2,true] result_1.bat obj
pause

echo.
echo. 2.����json�ļ�
echo. ���jsoner -f test.json result_2.bat test
jsoner -f test.json result_2.bat test

echo.
echo.������
pause
exit


