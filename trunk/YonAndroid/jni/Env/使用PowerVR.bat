@echo off
setlocal enabledelayedexpansion
echo ��ʼ�������Ժ�......

set in_dir=PowerVR\lib
set out_dir=C:\Windows\System32

cd /d %~dp0
copy %in_dir%\*.dll %out_dir%\*.dll
echo ��ɣ�ллʹ��
pause>nul