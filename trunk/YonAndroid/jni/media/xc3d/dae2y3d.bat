@echo off
echo ��ʼ�������Ժ�......
cd /d %~dp0
for /r %%a in (*.dae) do (
if not exist "%%~npa.y3d" (
echo ��ʼת�� %%a ...
dae2y3d.exe %%a %%~na.y3d
)
)
echo ��ɣ�ллʹ��
pause>nul