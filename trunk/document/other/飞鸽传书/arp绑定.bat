@echo off
echo ��ʼARP��
set ip=192.168.0.101
set mac=00-0C-29-78-4D-8D
arp.exe -s  %ip% %mac%
echo �󶨽���,�����ǽ��:
arp.exe /a
pause