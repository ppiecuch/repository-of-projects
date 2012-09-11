/**
NSString的stringWithFormat方法：
+ (id) stringWithFormat: (NSString*) format,...;

stringWithFormat方法中有两个值得讨论的地方：
1)、定义在最后的省略号，它告诉编译器这个方法可以接收多个以逗号隔开的其他参数。
2)、方法声明开始字符使用加号：
Objective-C运行时生成一个类的时候，它会创建一个代表该类的类对象。类对象包含了指向超类的指针、类名和指向类方法列表的指针。
类对象还包含一个long型数据，为新创建的类实例对象指定大小（以字节为单位）。
如果你在声明方法时添加了加号，那么就是把这个方法定义为类方法。这个方法属于类对象（不是类的实例对象），并且通常用于创建新的实例。
我们称这种用来创建新对象的类方法为工厂方法。

类方法也可以用来访问全局数据。AppKit中的NSColor类有一些以不同颜色命名的类方法如redColor和blueColor。要用蓝色绘图，可以这样：
NSColor *haveTheBlues=[NSColor blueColor];

我们所创建的大部分方法是实例方法，因此使用前导减号（-）来声明。

注意：类的静态创建方法创建的对象（比如 [NSString stringWithString: @"test"]）使用auto release，程序中需要有NSAutoreleasePool对象实例，否则运行时有日志警告信息
   autorelease called without pool for object (872a188) of class GSAutoreleasedMemory in thread <NSThread: 0x871bee8> 大致意思是说对象进行了autorelease却没有autorelease pool对象
auto release不定期执行，所有有时候已经release的对象还可以取到值

NSString中另一个方便的方法（实例方法）是length，它返回的是字符串中的字符个数：
- (unsigned int) length;
可以这样使用它：
unsigned int length=[height length];
也可以在表达式中使用
if([height length]>36)....
NSString的length方法能够准确无误地处理国际字符串，如含有俄文、中文或者日文，以及使用Unicode编码的字符串，相比之下在C语言中这种情况很让人头疼。

- (BOOL) isEqualToString:(NSString*) aString;
- (NSComparisonResult) compare:(NSString*) string;（区分大小写）
- (NSComparisonResult) compare:(NSString*) string options:(unsigned) mask;
mask：
    NSCaseInsensitiveSearch：不区分大小写字符
    NSliteralSearch：进行完全比较，区分大小写
    NSNumericSearch：比较字符串的字符个数，而不是字符值。

注意：使用==判断两个字符串只是判断两者的指针是否相等

- (BOOL) hasPrefix:(NSStrnig*) aString;
- (BOOL) hasSuffix:(NSString*) aString;

如果你想知道字符串内的某处是否包含其他字符串，请使用rangeOfString:

- (NSRange) rangeOfString:(NSString*) aString;

NSString是不可变的，这并不意味着你不能操作它们。不可变的意思是一旦NSString被创建了，我们就不能改变它。
Cocoa提供了一个NSString的子类，叫做NSMutableString。如果你想改变字符串，请使用这个子类。

+ (id) stringWithcapacity:(unsigned) capacity;
- (void) appendString:(NSString*) aString;
- (void) appendFormat:(NSString*) format;
- (void) deleteCharactersInRange:(NSRange) range;
*/

#import <Foundation/Foundation.h>

int main (int argc, const char *argv[])
{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

    NSString* height;
    height=[NSString stringWithFormat: @"Your height is %d feet,%d inches", 5, 11];

    NSLog(@"%@",height);

    NSString* filename=@"draft-chapter.pages";
    NSRange range;
    range = [filename rangeOfString: @"chapter"];
    NSLog(@"range:%d,%d",range.location,range.length);

    range = [filename rangeOfString: @"chapter1"];
    if(range.location==NSNotFound)
        NSLog(@"range:not found");

    NSMutableString* mStr;
    mStr=[NSMutableString stringWithCapacity:42];
    [mStr appendString: @"hello"];
    [mStr appendFormat: [NSString stringWithFormat: @"%@",@"mutable"]];
    NSLog(@"%@",mStr);

    [pool drain];

    return 0;
}

