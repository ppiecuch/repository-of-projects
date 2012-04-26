@echo off
echo 开始处理，请稍候......
for /f  %%a in ('dir /b *.*') do (
echo %%a >> log.txt
)
echo 完成！谢谢使用
pause>nul