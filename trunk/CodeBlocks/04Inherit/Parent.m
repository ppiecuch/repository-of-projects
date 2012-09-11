
#import "Parent.h"

@implementation Parent

- (void) setName : (NSString*) n
{
    name=n;
    NSLog(@"Parent set name:%@",n);
}

+ (void) staticTest
{
    NSLog(@"Parent static");
}
@end
