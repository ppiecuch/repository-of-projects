@echo off
echo 开始处理，请稍候......
set engine=obj\local\armeabi\objs\engine\Yon
set yon=obj\local\armeabi\objs\yon

del %engine%\*.o
del %engine%\*.o.d

rd /s /q %yon%

echo 完成！谢谢使用
pause>nul