/**
Objective-C有一种名为中缀符（infix notation）的语法技术。方法的名称及其参数都是合在一起的，这种语法的可读性更强。如：
[circle setColor:ENUM_RED];
[textThing setStringValue:@"hello" color:ENUM_GREEN];

方法声明中返回类型与参数类型都在圆括号中指定。
如果方法使用参数，则需要冒号，否则不需要。

最后@end告诉编译器，我们已经完成类的声明。

注意，方法的声明在括号后面，@end之前 {}区域里只有变量的定义，这点和C++很不一样。

如果类中没有包含任何实例变量，就可以省去花括号

setter方法根据它所更改的属性的名称来命名，并加上前缀“set”
getter方法则仅仅根据其返回的属性的名称来命名。不要将get用作getter方法的前缀。
说明：get这个词在Cocoa中有着特殊的含义。如果get出现在Cocoa的方法名称中，就意味着这个方法的参数是一些指针，经过处理后再将结果返回。如：

NSData的getBytes、NSBezierPath的getLineDash:count:phase


NSObject提供了一个名为respondsToSelector:的方法，该方法询问对象以确定其是否能够响应某个特定的消息。
if([car respondsToSelector:@selector(setEngine:)])....
*/

#ifndef _SHAPE_H_
#define _SHAPE_H_

#import <Foundation/Foundation.h>

typedef enum{
    ENUM_CIRCLE,
    ENUM_RECT
}ShapeType;

typedef enum{
    ENUM_RED,
    ENUM_GREEN,
    ENUM_BLUE
}ShapeColor;

@interface Shape : NSObject
{
    ShapeType shape;
    ShapeColor color;
}
- (ShapeColor) color;
- (void) setColor : (ShapeColor) color;
- (void) setShape : (ShapeType) shape;
- (void) draw;

@end

#endif
