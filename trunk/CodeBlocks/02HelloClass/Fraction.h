#ifndef FRACTION_H_INCLUDED
#define FRACTION_H_INCLUDED

/**
夹在@interface Class:Parent{...}中的称为instance variables.

Instance methods跟在成员变量(即instance variables)后.格式为
scope (returnType) methodName:(parameter1Type) parameter1Name;
1)、scope有class或instance两种。instance method以-开头，class level methods以+开头。

Interface以一个@end作为结束符。
*/

#import <Foundation/NSObject.h>

@interface Fraction: NSObject{
    int numerator;
    int denominator;
}

-(void) print;
-(void) setNumerator:(int) d;
-(void) setDenominator:(int) d;
-(int) numerator;
-(int) denominator;
@end


#endif // FRACTION_H_INCLUDED
