@echo off
echo 开始处理，请稍候......
cd /d %~dp0
for /r %%a in (*.dae) do (
if not exist "%%~npa.y3d" (
echo 开始转换 %%a ...
dae2y3d.exe %%a %%~na.y3d
)
)
echo 完成！谢谢使用
pause>nul