/**
NSArray和NSDictionary只能存储对象，而不能直接存储任何基本类型的数据，如int,float或struct。但是你可以用对象来封闭基本数值。

Cocoa提供了NSNumber类来包装（即以对象形式实现）基本数据类型。你可以使用下列类方法创建新的NSNumber对象：

+ (NSNumber*) numberWithChar:(char) value;
+ (NSNumber*) numberWithInt:(int) value;
+ (NSNumber*) numberWithFloat:(float) value;
+ (NSNumber*) numberWithBool:(BOOL) value;
还在许多这种创建方法，包括无符号版本和各种long型数据及long long整型数据。

- (char) charValue;
- (int) intValue;
- (float) floatValue;
- (BOOL) boolValue;
- (NSString*) stringValue;

NSNumber实际上是NSValue的子类，NSValue可以包装任意值。你可以用NSValue将结构放入NSArray和NSDictionary中。

+ (NSValue*) valueWithBytes:(const void*) value objCType:(const char*) type;
+ (NSValue*) valueWithPoint:(NSPoint) point;
+ (NSValue*) valueWithSize:(NSSize) size;
+ (NSValue*) valueWithRect:(NSRect) rect;
- (void) getValue:(void*) value;
- (NSPoint) pointValue;
- (NSSize) sizeValue;
- (NSRect) rectValue;

*/

#include <iostream>

using namespace std;

int main()
{
    cout << "Hello world!" << endl;
    return 0;
}
