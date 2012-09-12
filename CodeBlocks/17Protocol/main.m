/**
@interface Car:NSObject <NSCopying,NSCoding>
...

Objective-C2.0支持@required/@optional来定义协议中哪些方法是必须的，哪些方法是可选的
*/
#import <Foundation/Foundation.h>

@protocol NSP1
- (void) test1;
@end

@protocol NSP2
- (void) test2;
@end

@interface Test : NSObject <NSP1,NSP2>
@end

@implementation Test
- (void) test1
{
    NSLog(@"test1");
}
- (void) test2
{
    NSLog(@"test2");
}
@end

int main (int argc, const char *argv[])
{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

    id test=[Test new];
    [test test1];
    [test test2];
    [test release];

    [pool drain];

    return 0;
}

