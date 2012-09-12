/**
分配（allocation）是从操作系统获得一块内在并将其指定为存放对象的实例变量的位置。
向某个类发送alloc消息的结果，是为该类分配一块足够大的内存，以存放该项类的全部实例变量。同时，
alloc方法还顺便将这块内存区域全部初始化为0。


实例变量所在的内在位置到隐藏的self参数之间的距离是固定的。
self=[super init]
这个赋值操作只影响该init方法中self的值，而不影响该方法范围以外的任何内容（不明？）

NSString:
- (id) init;
- (id) initWithFormat:(NSString*) format,...;
- (id) initWithContentsOfFile:(NSStrnig*) path;

在支持垃圾回收的Objective-C中，dealloc方法不会执行，如果需要在销毁对象时执行某些操作，则需要重写-finalize方法。

不需要为你自己的类创建初始化函数方法。如果不需要设置任何状态，或者只需要alloc方法将内存清零的默认行为，则不需要担心init。
如果构造了一个初始化函数，则一定要在你自己的指定初始化函数中调用超类的指定初始化函数。
如果初始化函数不止一个，则要选择一个作为指定初始化函数。被选定的方法应该调用超类的指定初始化函数。要按照指定初始化函数的形式实现所有其他化函数。
*/

#include <iostream>

using namespace std;

int main()
{
    cout << "Hello world!" << endl;
    return 0;
}
