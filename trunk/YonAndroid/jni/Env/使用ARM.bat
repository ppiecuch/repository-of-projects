@echo off
setlocal enabledelayedexpansion
echo ��ʼ�������Ժ�......

set in_dir=.\ARM\lib
set out_dir=C:\Windows\System32
xcopy %in_dir%\*.dll %out_dir%\*.dll /E /Y
echo ��ɣ�ллʹ��
pause>nul