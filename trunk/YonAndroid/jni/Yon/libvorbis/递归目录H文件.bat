@echo off
echo ��ʼ�������Ժ�......
for /f  %%a in ('dir /b /a /s *.h') do (
echo %%~nxa >> log.txt
)
echo ��ɣ�ллʹ��
pause>nul