@echo off
echo 开始处理，请稍候......
for /r %%a in (*.dae) do (
dae2y3d.exe %%a %%~na.y3d
)
echo 完成！谢谢使用
pause>nul