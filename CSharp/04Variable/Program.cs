using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

//refer to:http://www.sufeinet.com/thread-3121-1-1.html
//refer to:http://www.sufeinet.com/thread-3269-1-1.html
//refer to:http://www.ibcibc.com/thread-934-1-1.html


//定义
//被用来存储特定类型的数据，可以根据需要随时改变所存储的数据值。具有名称，类型和值这就是变量

//值类型
//1.值类型变量都存储在堆栈中。
//2.访问值类型变量时，一般都是直接访问其实例。
//3.每个值类型变量都有自己的数据副本，因此对一个值类型的变量的操作不会影响其他的变量。
//4.复制值类型变量时，复制的是变量的值，而不是变量的地址。
//5.值类型变量不能为null，必须具有一个确定的值。

//引用类型
//1.必须在托管堆中为引用类型变量分配内存。
//2.必须使用new关键字来创建引用类型变量。
//3.在托管堆中分配的每个对象都有与之相关联的附加成员，这些成员必须被初始化。
//4.引用类型变量是由垃圾回收机制来管理的。
//5.多个引用类型变量都可以引用同一个对象，这种情形下，对一个变量的操作会影响另一个变量所引用的同一对象。
//6.引用类型被赋值之前的值都是null。

//1.定义变量时，数据类型后面添加“?”
//在变量类型后添加“?”表示可空值类型。
//例如:
//int a = null;//错误
//int? a = null;//正确
//此时，变量a可以为空(null)，并且可以通过它的 HasValue 属性来判断是否有值，并通过 Value 属性获得真正的值。
//它可以通过对包装类型直接赋值的方式隐式创建，如：
//int? a = 50;
//如Int32、结构体等这样的类型是不可能为空的，常见的如int,DateTime都是不可能为空的。所以在需要判断是否有值的场合，可以用加问号的可空值类型，如int?,DateTime?等


//初始化
//C#中变量是一种强类型，也可以理解为强制定义类型，强制安全性。他的意思就是说在使用这个变量之前必须先规定他的类型以保证安全性。
//变量在使用之前必须指定类型并初始化。

//类型推断
//这类其实主要是一个关键字var，推断的工作是由编译器来做的，因为Ｃ＃中保留了var进行变量的初始化与定义。
//这个与JS的变量有些相假似，我们正常的定义方法是强类型，就是说会根据变量的类型来确定值的类型。
//然而使用Var正好相反,它是根据值的变量来确实变量的类型。


//常量语法如下：
//const 类型标识符 常量名 = 表达式；
//常量定义：在编译时其值能够确定，并且程序运行过程中值不发生变化的量。定义常量类型有int,bool,char,double,sting等。

//变量的作用域：
//声明变量的地方有两种（方法体外），（方法体中）。在类中声明变量有时称为字段或成员变量。
//public 不限制访问。
//protected 仅限于当前类和派生类。
//Internal 访问仅限于当前程序集。
//protected internal 访问仅限于当前程序集或从当前派生的类。
//private 仅限于当前类。



namespace _04Variable
{
    class Program
    {
        static void Main(string[] args)
        {
            Student s = new Student
            {
                name = "Micheal",
                age = 24
            };
            //这就是引用类型
            Student t = s;
            t.age = 28;
            Console.WriteLine("name={0},age={1}", s.name, s.age);
            

            //int a = null; //错误 CS0037: 无法将 Null 转换成“int”，因为它是一种不可为 null 值的类型
            int? a = null;
            //Console.WriteLine("a.HasValue:{0},a.Value:{1}", a.HasValue, a.Value);//会崩
            Console.WriteLine("a.HasValue:{0}", a.HasValue);
            a = 30;
            Console.WriteLine("a.HasValue:{0},a.Value:{1}", a.HasValue, a.Value);

            //var tmp;//错误 CS0818: 隐式类型的局部变量必须已初始化
            //Console.WriteLine("tmp的类型：" + tmp.GetType().ToString());
            var name = "苏飞";
            var age = 26;
            var isRabbit = false;
            Console.WriteLine("name的类型：" + name.GetType().ToString());
            Console.WriteLine("age的类型：" + age.GetType().ToString());
            Console.WriteLine("isRabbit的类型：" + isRabbit.GetType().ToString());


            Console.ReadKey();
        }
    }
}
