/**
NS: Cocoa的前身是NextStep

Objective-C是C的衍生语言，继承了所有C语言特性。

Xcode是苹果公司向开发人员提供的集成开发环境（非开源），用于开发Mac OS X的应用程序。
Xcode的.m扩展名表示文件含有Objective-C代码，应由Objective-C编译器处理。

#import是GCC编译器提供的，Xcode在编译Objective-C、C和C++程序时都会使用它。#import可保证头文件只被包含一次，不论此命令出现多少次。
依赖关系是两个实体之间的一种关系，导入头文件在头文件和进行导入的源文件之间建立了一种很强的依赖关系，这导致当头文件变更时，导入的源文件需要
重新编译来适应变化，部分情况下可以只声明类名来减少这些依赖。（@class YourClass;）
@class创建了一个前向引用。

苹果公司将Cocoa、Carbon、QuickTime和Opengl等技术作为框架集提供。Cocoa的组成部分有Foundation和Application Kit(也称为App Kit)框架。
还有一个支持框架的套件，包含Core Animation和Core Image，这为Cocoa增添了多种精彩功能。
使用#import <Foundation/Foundation.h>就能够获得整个集合。

@符号是Objective-C在标准C语言基础上添加的特性之一。
双引号中的字符串前有一个@符号，这表示引用的字符串应该作为Cocoa的NSString元素来处理。
*/
#import <Foundation/Foundation.h>
int main (int argc, const char *argv[])
{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    NSLog(@"%@",@"hello objectc");
    [pool drain];

    return 0;
}
