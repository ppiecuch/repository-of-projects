@echo off
echo ��ʼ�������Ժ�......
cd /d %~dp0
for /r %%a in (*.dae) do (
if not exist "%%~npa.xc3d" (
echo ��ʼת�� %%a ...
dae2xc3d.exe -i %%a -o %%~na.xc3d
)
)
echo ��ɣ�ллʹ��
pause>nul