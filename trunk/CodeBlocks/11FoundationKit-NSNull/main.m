/**
NSNull
+ (NSNull*) null;
[NSNull null]总是返回一样的数值，所以你可以使用运算符==将该值与其他值进行比较。

[contact setObject:[NSNull null] forKey:@"test"];
id homefax=[contact objectForKey:@"test"];
if(homefax==[NSNull null])....
*/

#include <iostream>

using namespace std;

int main()
{
    cout << "Hello world!" << endl;
    return 0;
}
