@echo 将所有文件复制到这个目录下 ：src-1.11.1

　　pause

　　md .\src-1.11.1

　　copy .\libiconv-1.11.1\lib\* .\src-1.11.1\

　　copy .\libiconv-1.11.1\include\IConv.h.in .\src-1.11.1\iconv.h

　　copy .\libiconv-1.11.1\srcLib\localcharset.h .\src-1.11.1\

　　copy .\libiconv-1.11.1\libcharset\include\localcharset.h.in .\src-1.13.\

　　copy .\libiconv-1.11.1\libcharset\lib\localcharset.c .\src-1.13.\

　　ren .\src-1.11.1\Config.h.in Config.h

　　ren .\src-1.11.1\localcharset.h.in localcharset.h

　　@echo 目录文件生成成功。

　　pause

　　OK 以上完成了全部文件的准备（在src-1.11.1下）。 