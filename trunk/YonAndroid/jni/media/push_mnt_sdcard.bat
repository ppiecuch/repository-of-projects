@echo off
setlocal enabledelayedexpansion

set output=/mnt/sdcard/media

cd /d %~dp0
for /f "delims=" %%a In ('"dir * /a/b/s|findstr /v ".svn" "') do (
set name=%%~fa
set relative=!name:%cd%\=!
set relative=!relative:\=/!
if exist !name!\nul (
adb shell mkdir %output%/!relative!
) else (
adb push !name! %output%/!relative!
)
)
pause