/**
Cocoa采用引用计数技术来管理对象的生命周期。
当使用alloc,new方法或者通过copy消息（生成接收对象的一个副本）创建一个对象时，对象的保留计数器值被设置为1。
要增加对象的保留计数器值，可以给对象发送一条retain消息；要减少对象的保留计数器值，可以给对象发送一条release消息。

当一个对象因其保留计数器归0而即将被销毁时，Objective-C自动向对象发送一条dealloc消息（其结果是调用dealloc方法）。你可以在自己的对象中重写dealloc方法。
可以通过这种方法释放已经分配的全部相关资源。一定不要直接调用dealloc方法。

- (id) retain;
- (void) release;
- (unsigned) retainCount;


访问方法的保留和释放
- (void) setEngine:(Engine*) newEngine
{
    [newEngine retain];
    [engine release];
    engine=newEngine;
}


所有对象全部入池
Cocoa中有一个自动释放池（autorelease pool）
NSObject类提供了一个autorelease方法：
- (id) autorelease;
该方法预先设定了一条在将来某个时间发送的release消息，其返回值是接收消息的对象。
当给一个对象发送autorelease消息时，实际上是将该对象添加到NSAutoreleasePool中。当自动释放池被销毁时，会向该池中的所有对象发送release消息。
[pool release]
[pool drain]//该方法只是清空自动释放池而不销毁它。-drain只适用于Mac OS X10.4(Tiger)及更高版本。

临时对象
array=[[NSMutableArray alloc] init];
...
[array release];
相对地
array=[NSMutableArray arrayWithCapacity:17];则不需要release（可以当作被autorelease）

需要注意的是，有时，自动释放池未能按照预期的情况进行清理，如创建大量临时对象。

在编写iPhone程序时，苹果公司建议你不要在自己的代码中使用autorelease方法，同时还要避免使用创建自动释放对象的便利函数。
*/

#import <Foundation/Foundation.h>

@interface RetainTracker:NSObject
@end

@implementation RetainTracker

- (id) init
{
    if(self=[super init])
        NSLog(@"init:refer count:%d",[self retainCount]);
    return self;
}

- (void) dealloc
{
    NSLog(@"dealloc");
    [super dealloc];
}

@end

int main (int argc, const char *argv[])
{
    RetainTracker* tracker=[RetainTracker new];
    [tracker retain];
    NSLog(@"%d",[tracker retainCount]);
    [tracker release];
    NSLog(@"%d",[tracker retainCount]);
    [tracker retain];
    NSLog(@"%d",[tracker retainCount]);
    [tracker release];
    NSLog(@"%d",[tracker retainCount]);
    [tracker release];

    return 0;
}
