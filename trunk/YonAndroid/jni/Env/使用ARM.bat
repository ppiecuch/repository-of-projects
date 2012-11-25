@echo off
setlocal enabledelayedexpansion
echo 开始处理，请稍候......

set in_dir=ARM\lib
set dll_dir=C:\Windows\System32
set lib_dir="C:\Program Files\Microsoft Visual Studio 9.0"\VC\lib

cd /d %~dp0
copy %in_dir%\*.lib %lib_dir%\*.lib
copy %in_dir%\*.dll %dll_dir%\*.dll
echo 完成！谢谢使用
pause>nul