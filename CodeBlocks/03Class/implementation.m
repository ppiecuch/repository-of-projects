/**
implementation中包含全局变量的定义、私有struct以及声明方法的实现
如果使用.mm做文件扩展名，编译器就会认为你是用Objective-C++编写代码，这样你就可以同时使用C++语言和Objective-C来编程了。

@implementation是一个编译器指令，表明你将为某个类提供实现。
类名出现在implementation之后，该行没有分号，因为Objective-C编译器指令不需要分号。

我们甚至可以在实现中定义那些在interface中未声明的方法，可以把它们看成私有方法，仅在类的实现中使用。

将color/shape重命名为c/t是为了避免冲突。

在Objective-C中调用方法时，一个名为self的隐藏参数将被传递给接收对象，而这个参数引用的就是该接收对象。例如，在代码
[circle setColor:ENUM_RED]中，方法将circle作为其self参数进行传递。

init方法返回的值是id类型，即泛型对象指针
由于Objective-C中，实例变量所在的内存位置到隐藏的self参数之间的距离是固定的。
因此，如果从init方法返回一个新对象，则需要更新self，以便其后的任何实例变量的引用可以被映射到正确的内存位置。
第一行 if（self = [super init])（注意是赋值运算不是等于判断）
这段代码暗示self对象可能发生变化。该声明中最先执行的是 ［super init］，其作用是使super超类完成它们自己的初始化工作。

description方法
*/

#import "interface.h"

@implementation Shape

NSString* shapeName(ShapeType t)
{
    switch(t)
    {
    case ENUM_CIRCLE:
        return (@"Circle");
    case ENUM_RECT:
        return (@"Rect");
    }
    return (@"Unknown");
}

NSString* colorName(ShapeColor c)
{
    switch(c)
    {
    case ENUM_RED:
        return @"Red";
    case ENUM_GREEN:
        return @"Green";
    case ENUM_BLUE:
        return @"Blue";
    }
    return @"Unknown";
}

- (id) init
{
    if(self=[super init])
        NSLog(@"init");
    return self;
}

- (NSString*) description
{
    return @"ShapeDescription";
}

//getter
- (ShapeColor) color
{
    return color;
}
//setter
- (void) setColor:(ShapeColor) c
{
    color=c;
}

- (void) setShape:(ShapeType) t
{
    shape=t;
}

- (void) draw
{
    NSLog(@"%@ draw a %@ in %@",self,shapeName(shape),colorName(color));
}

@end
