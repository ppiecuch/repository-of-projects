/**
NSArray是一个Cocoa类，用来存储对象的有序列表。你可以在NSArray中放入任意类型的对象。
NSArray有两个限制。首先，它只能存储Objective-C的对象，而不能存储C语言中的基本数据类型，如int、float、enum、struct，或者NSArray中的随机指针。
同时，你也不能在NSArray中存储nil（对象的零值或NULL值）。
可以通过类方法arrayWithObjects：创建一个新的NSArray。发送一个以逗号分隔的对象列表，在列表结尾添加nil代表列表结束。

- (unsigned) count;
- (id) objectAtIndex:(unsigned int) index;

使用-componentsSeparatedByString：来切分NSArray
用componentsJoinedByString:来合并NSArray中的元素并创建字符串

与NSString一样，NSArray创建的确是不可变对象的数组。一旦你用特定数量的对象创建了一个数组，那么它就固定下来了：你既不能添加任何元素也不能删除任何元素。
当然数组中包含的对象是可以改变的，但数组对象本身是一直都不会改变的。

于是出现了NSArray的补充类NSMutableArray
+ (id) arrayWithCapacity:(unsigned) numItems;
- (void) addObject:(id) anObject;
- (void) removeObjectAtIndex:(unsigned) index;
*/

#import <Foundation/Foundation.h>

int main (int argc, const char *argv[])
{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

    NSArray* array;
    array = [NSArray arrayWithObjects:@"one",@"two",@"three",nil];

    int i;
    for (i=0;i<[array count];++i)
        NSLog(@"array[%d]:%@",i,[array objectAtIndex:i]);

    [pool drain];
    return 0;
}
