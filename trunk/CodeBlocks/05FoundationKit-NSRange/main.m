/**
Cocoa为我们准备了一些结构体，如NSRange:
typedef struct _NSRange{
    unsigned int location;
    unsigned int length;
}NSRange;

类似的，还有NSPoint,NSSize,NSRect
typedef struct _NSPoint{
    float x;
    float y;
}NSPoint;
typedef struct _NSSize{
    float width;
    float height;
}NSSize;
typedef struct _NSRect{
    NSPoint origin;
    NSSize size;
}NSRect;

同样的，他们也有快捷函数：NSMakePoint(),NSMakeSize(),NSmakeRect()
*/

#import <Foundation/Foundation.h>

int main (int argc, const char *argv[])
{
    //可以用3种方式创建NSRange
    //第一种：
    NSRange range1;
    range1.location=17;
    range1.length=4;

    NSLog(@"range1:%d,%d",range1.location,range1.length);

    //第二种：
    NSRange range2 = {17, 4};

    NSLog(@"range2:%d,%d",range2.location,range2.length);

    //第三种：
    NSRange range3 = NSMakeRange(17,4);
    //推荐使用NSMakeRange

    NSLog(@"range3:%d,%d",range3.location,range3.length);

    return 0;
}

