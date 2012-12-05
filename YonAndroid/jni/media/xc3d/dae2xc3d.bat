@echo off
echo 开始处理，请稍候......
cd /d %~dp0
for /r %%a in (*.dae) do (
if not exist "%%~npa.xc3d" (
echo 开始转换 %%a ...
dae2xc3d.exe -i %%a -o %%~na.xc3d
)
)
echo 完成！谢谢使用
pause>nul