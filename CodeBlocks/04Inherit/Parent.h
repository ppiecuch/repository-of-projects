
#ifndef _PARENT_H_
#define _PARENT_H_

/**
Objective-C中所有对象间的交互都是通过指针实现的。

在设计过程中，到底使用继承还是使用复合可以用一个规则来判断："is a"和"has a"

注意：与实例方法一样，继承对类方法同样适用
*/

#import <Foundation/Foundation.h>

@interface Parent:NSObject
{
    NSString* name;
}

- (void) setName : (NSString*) name;
+ (void) staticTest;

@end
#endif
