#import "Child.h"

int main (int argc, const char *argv[])
{
    id c=[Child new];
    [c setName:@"micheal"];

    [Child staticTest];

    return 0;
}
