/**
Objective-C是C的衍生语言，继承了所有C语言特性。
*/
#import <Foundation/Foundation.h>
int main (int argc, const char *argv[])
{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    NSLog(@"%@",@"hello objectc");
    [pool drain];

    return 0;
}
