@echo off
setlocal enabledelayedexpansion
echo 开始处理，请稍候......
cd /d %~dp0
for /f  %%a in ('dir /b /a-d *.*') do (
rem adb push %%~nxa /storage/sdcard0/media
adb push %%~nxa /mnt/sdcard/media
rem echo %%~nxa
)
echo 完成！谢谢使用
pause>nul