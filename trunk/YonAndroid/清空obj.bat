@echo off
echo ��ʼ�������Ժ�......
set engine=obj\local\armeabi\objs\engine\Yon
set yon=obj\local\armeabi\objs\yon

del %engine%\*.o
del %engine%\*.o.d

rd /s /q %yon%

echo ��ɣ�ллʹ��
pause>nul