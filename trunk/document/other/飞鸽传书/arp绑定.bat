@echo off
echo 开始ARP绑定
set ip=192.168.0.101
set mac=00-0C-29-78-4D-8D
arp.exe -s  %ip% %mac%
echo 绑定结束,以下是结果:
arp.exe /a
pause