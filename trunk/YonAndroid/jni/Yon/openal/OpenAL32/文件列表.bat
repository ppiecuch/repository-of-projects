@echo off
echo ��ʼ�������Ժ�......
for /f  %%a in ('dir /b *.*') do (
echo %%a >> log.txt
)
echo ��ɣ�ллʹ��
pause>nul