@echo off
setlocal enabledelayedexpansion
echo ��ʼ�������Ժ�......
cd /d %~dp0
for /f  %%a in ('dir /b /a-d *.*') do (
rem adb push %%~nxa /storage/sdcard0/media
adb push %%~nxa /mnt/sdcard/media
rem echo %%~nxa
)
echo ��ɣ�ллʹ��
pause>nul