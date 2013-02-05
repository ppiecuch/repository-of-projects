一个linux下面分析Android崩溃日志(crash log)的工具。把崩溃的日志信息(带I/DEBUG标记)复制到xx文件中，用编辑器编辑工具中Android编译的符号(symbol)所在的位置，我的是在309行"SYMBOLS_DIR = './out/target/product/sirfsocv7/symbols’"，保存，然后执行./stack xx 就可以分析出出现问题代码的调用关系。

http://download.csdn.net/download/wangmxe/4522459