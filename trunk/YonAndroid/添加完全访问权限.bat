@echo off
echo ��ʼ�������Ժ�......
for /f  %%a in ('dir /b *.a') do (
icacls %%a /grant:r "Everyone":F
)
echo ��ɣ�ллʹ��
pause>nul