/**
由于对象的局部变量特定于该对象的实例，因此我们称它们为实例变量，通常简称为ivars。

为了创建新对象，我们需要向相应的类发送new消息。

在Objective-C 中，id 类型是一个独特的数据类型。在概念上，类似Java 的Object 类，可以转换为任何数据类型。

*/

#import "interface.h"

void drawShapes(id shapes[], int count)
{
    int i;
    for(i=0;i<count;++i)
    {
        id shape=shapes[i];
        [shape draw];
    }
}

int main (int argc, const char *argv[])
{
    id shapes[2];

    shapes[0]=[Shape new];
    [shapes[0] setColor:ENUM_RED];
    [shapes[0] setShape:ENUM_CIRCLE];

    shapes[1]=[Shape new];
    [shapes[1] setColor:ENUM_GREEN];
    [shapes[1] setShape:ENUM_RECT];

    drawShapes(shapes,2);

    NSLog(@"color:%d",[shapes[0] color]);
    [shapes[0] setColor:ENUM_BLUE];
    NSLog(@"color:%d",[shapes[0] color]);

    return 0;
}
