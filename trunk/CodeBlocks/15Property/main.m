/**
@prperty只不过是给编译器看的一种指令，它可以编译之后为你生成相应的getter和setter方法。

@property(nonatomic, retain) UITextField *userName;             //1
@property(nonatomic, retain,readwrite) UITextField *userName;   //2
@property(atomic, retain) UITextField *userName;                //3
@property(retain) UITextField *userName;                        //4
@property(atomic,assign) int i;                                 //5
@property(atomic) int i;                                        //6
@property int i;                                                //7
上面的代码1和2是等价的，3和4是等价的，5,6,7是等价的。也就是说atomic是默认行为，assign是默认行为，readwrite是默认行为。

atomic和nonatomic的作用与区别：
使用atomic，保证了某种程度上的线程安全。但是，要写出线程安全的代码，还需要有同步和互斥机制。
很明显，nonatomic比atomic速度要快。


@synthesize也是一种新的编译器功能，表示“创建该属性的访问器”。
@synthesize name=appllation;编译器仍将创建-setName和-name方法，但在实现中却使用实例变量appellation;
*/


#import <Foundation/Foundation.h>

@interface Test : NSObject
{
    int temp;
}
@property int temp;
@end

@implementation Test
@end

int main (int argc, const char *argv[])
{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

    id test=[Test new];
    //[test setTemp:1];
    NSLog(@"%d",[test temp]);
    NSLog(@"%@",[test description]);
    [test release];

    [pool drain];
    return 0;
}
