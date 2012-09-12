/**
字典就是关键字及其定义的集合。
NSDictionary在给定的关键字（通常是一个NSString字符串）下存储一个数值（可以是任何类型的对象）。

+ (id) dictionaryWithObjectsAndKeys:(id) firstObject,...;
- (id) objectForKey:(id) aKey;

同样地，NSDictionary是不可变的对象，相对地NSMutableDictionary类允许你随意添加和删除字典元素。

+ (id) dictionaryWithCapacity:(unsigned int) numItems;
- (void) setObject:(id) anObject forKey:(id) aKey;
- (void) removeObjectForKey:(id) aKey;

*/

#import <Foundation/Foundation.h>

int main (int argc, const char *argv[])
{
    return 0;
}
