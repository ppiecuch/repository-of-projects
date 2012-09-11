/**
C语言拥有布尔数据类型bool，它具有true值和false值。Objective-C提供了相似的类型BOOL，它具有YES和NO值。
这两种布尔类型可以在同一个程序中共存，但在编写Cocoa代码时要使用BOOL。

注意：直接使用 return (n1-n2)虽然在C语言中可以得到真值，但在Objective-C中1并不等于YES。

格式化字符串中%@表示对应的是一个NSString指针（事实上可以是任何NS对象，因为NSObject有一个description方法）
*/
#import <Foundation/Foundation.h>

BOOL areIntsDifferent(int n1,int n2)
{
    if(n1==n2)
        return (NO);
    else
        return (YES);
}

NSString *boolString(BOOL yn)
{
    if(yn==NO)
        return (@"NO");
    else
        return (@"YES");
}


int main (int argc, const char *argv[])
{
    BOOL result;
    result=areIntsDifferent(5,5);
    NSLog(@"result of areIntsDifferent(%d,%d)->%@",5,5,boolString(result));

    result=areIntsDifferent(5,6);
    NSLog(@"result of areIntsDifferent(%d,%d)->%@",5,6,boolString(result));

    return 0;
}
