@echo off
echo ��ʼ�������Ժ�......
for /r %%a in (*.dae) do (
dae2y3d.exe %%a %%~na.y3d
)
echo ��ɣ�ллʹ��
pause>nul