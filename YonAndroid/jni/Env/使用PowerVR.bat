@echo off
setlocal enabledelayedexpansion
echo 开始处理，请稍候......

set in_dir=PowerVR\lib
set out_dir=C:\Windows\System32

cd /d %~dp0
copy %in_dir%\*.dll %out_dir%\*.dll
echo 完成！谢谢使用
pause>nul