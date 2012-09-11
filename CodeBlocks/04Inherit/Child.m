
#import "Child.h"

@implementation Child

- (void) setName : (NSString*) n
{
    [super setName:n];
    NSLog(@"Child set name:%@",n);
}

+ (void) staticTest
{
    [super staticTest];
    NSLog(@"Child static");
}

@end
