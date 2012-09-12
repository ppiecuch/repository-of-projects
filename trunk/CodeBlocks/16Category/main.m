/**
利用Objective-C的动态运行时分配机制，你可以为现有的类添加新方法。这些新方法的Objective-C术语称为“类别”（category）

需要保证类别名称的唯一性，你可以向一个类中添加任意多的类别。
由于不能添加新实现变量，因此与类的声明不同的是，类别的声明中没有实例变量部分。

类别的局限性
第一、无法向类中添加新的实例变量。类别没有位置容纳实例变量；
第二、名称冲突，即类别中的方法与现有方法重名时，类别具有更高的优先级。你的类别方法将完全取代初始方法，从而无法再使用初始方法。

委托强调类别的另一种应用：被发送给委托对象的方法可以声明为一个NSObject的类别。
*/

#import <Foundation/Foundation.h>

@interface NSString (NumberConvenience)
- (NSNumber*) lengthAsNumber;
@end

@implementation NSString(NumberConvenience)
- (NSNumber*) lengthAsNumber
{
    unsigned int length=[self length];
    return ([NSNumber numberWithUnsignedInt:length]);
}
@end

int main (int argc, const char *argv[])
{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

    NSMutableDictionary* dict=[NSMutableDictionary dictionary];
    [dict setObject:[@"hello" lengthAsNumber] forKey:@"hello"];
    [dict setObject:[@"category" lengthAsNumber] forKey:@"category"];
    NSLog(@"%@",dict);

    [pool drain];
    return 0;
}
