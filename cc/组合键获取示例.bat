@echo off

echo.1.�鿴����
echo.���� cc ?
cc ?
pause

echo.2.�ȴ���ȡ������ϣ�q �� w+e �� ctrl+s �� ctrl+alt+d �� z+x+c
echo.������ֵΪ1���ȡq������ֵΪ2���ȡ��w+e��
echo.������ֵΪ3���ȡ��ctrl+s������ֵΪ4���ȡ��ctrl+alt+d
echo.������ֵΪ��ȡ��z+x+c
echo.���� cc K "81 87+69 17+83 17+18+68 90+88+67"
cc K "81 87+69 17+83 17+18+68 90+88+67"
echo.����ֵ:%errorlevel%
echo.ע�⣬�ȴ���ȡ�İ�������У�������һ���������һ������Ӽ������
echo.�粻��"81 81+69" ����"81+87 81+87+69"
echo.���ǿ���"81+87 81+69"
pause
cc 100
echo.3.��ȡ�������������������Ӧ��ֵ������ֵΪ��Ӧ��ֵ
echo.���� cc K _
:loop
cc K _
echo.����ֵ:%errorlevel%
goto:loop

pause
exit