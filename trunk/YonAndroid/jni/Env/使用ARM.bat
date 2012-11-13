@echo off
setlocal enabledelayedexpansion
echo 开始处理，请稍候......

set in_dir=.\ARM\lib
set out_dir=C:\Windows\System32
xcopy %in_dir%\*.dll %out_dir%\*.dll /E /Y
echo 完成！谢谢使用
pause>nul