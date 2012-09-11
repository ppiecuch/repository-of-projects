
#ifndef _CHILD_H_
#define _CHILD_H_

/**
Objective-C提供某种方式来重写方法，并且仍然调用超类的实现方式。为了调用继承方法的实现，需要使用super作为方法调用的目标。
*/

#import "Parent.h"

@interface Child:Parent
{

}

@end

#endif
