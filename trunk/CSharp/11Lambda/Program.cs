using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Linq.Expressions;

//refer to:http://www.cnblogs.com/smwikipedia/archive/2009/05/06/1450825.html

//Lambda简介
//一个Lambda Expression  (译为Lambda式) 就是一个包含若干表达式和语句的匿名函数。可以被用作创建委托对象或表达式树类型。
//所有的Lambda式都使用操作符“=>“，表示“goes to (转变为)”。操作符左边部分是输入参数表，右边部分是表达式或语句块。x => x * x 读成“x转变为x 乘x”。


//所有对于匿名方法的约束也同样适用于Lambda式。


//表达式Lambda 
//由一个计算表达式组成的一个Lambda式称之为表达式Lambda。表达式Lambda常被用于构造表达式树。一个表达式Lambda返回计算表达式运算的结果。 
//基本结构：
//(input parameters) => expression
//如果只有一个输入参数时，括号可以省略。如果具有一个以上的输入参数，必需加上括号。
//(x) => x * x 等于 x => x * x 
//(x, y) => x == y
//可以显式指定输入参数的类型
//(int x, string s) => s.Length > x
//也可以没有任何输入参数
//() => SomeMethod1()
//上面这段代码在Lambda式中调用了一个方法。需要注意的是，如果在创建会被其他方使用的表达式树的时候，不宜在Lambda式中执行方法调用。比如：在SQL Server内执行。
//一般来说，让一个方法在原先设计的上下文环境以外执行没有意义，也不能真正工作。


//语句Lambda
//语句Lambda和表达式Lambda非常相似，只是语句被包含在大括号内：
//(input parameters) => {statement;}
//大括号中的语句可以是任意多条，也可以写成多行（定义一个Lambda式也就是在定义一个匿名方法）：
//TestDelegate myDel = n => { string s = n + " " + "World"; 
//                          Console.WriteLine(s); };
//当然语句Lambda跟匿名方法一样，无法用于创建表达式树。


//类型猜测
//当编写一个Lambda式的时候，我们通常不需要明确指定输入参数的类型。因为编译器会根据Lambda体的实现，以及委托的定义来猜测类型。
//举例：如果要从一个List<int>中删除小于100的元素
//lst.RemoveAll(i => i < 100);       //i会被猜测为int


//通常的猜测规则如下：
//· Lambda式必须包含与委托定义中相等数量的输入参数；
//· 每个Lambda式的输入参数必须能够隐式转换成委托定义中所要求的输入参数；
//· Lambda式的返回值必须能够隐式转换成委托定义中的返回值。
//注意：由于目前在common type system中还没有一个“Lambda式类型”的类型。如果在有些场合提到“Lambda式的类型”，那通常表示委托的定义或者是Expression<>类型。


//Lambda式中的变量作用域
//在Lambda式定义中可以引用外部变量。只要是在定义处能够访问到的变量，都可以在Lambda式中引用。
//Lambda式的定义仅仅是定义一个匿名方法，最终会生成一个委托对象。外部变量的引用将被“捕获”到委托对象内部，将会伴随委托对象的整个生命周期。
//在委托对象生命周期结束之前该变量都不会被垃圾回收。就算外部变量已经超过了原来的作用域，也还能继续在Lambda式中使用。所有会被引用的外部变量必须在Lambda式定义之前被显式赋值。


//下面是关于变量作用域的规则：
//·被“捕获”的变量在委托的生命周期结束前都不会被垃圾回收；
//·在Lambda式内部定义的变量对外不可见；
//·Lambda式无法直接捕获一个具有ref或out描述的参数变量；（如果使用了会怎样？）
//·Lambda式中的return语句不会导致当前所在的方法返回；(什么意思？)
//·Lambda式中不允许包含会导致跳当前执行范围的goto，break 或 continue语句。

//总结
//Lambda式可以说就是另外一种形式的匿名方法。用在某些地方，会使代码更加简洁。
//定义一个Lambda式本质上就是定义一个委托的实现体。



namespace _11Lambda
{

    delegate bool D();
    delegate bool D2(int i);


    class Program
    {
        delegate int dele(int i);

        D del;
        D2 del2;

        public void TestMethod(int input)
        {
            int j = 0;

            // Initialize the delegates with lambda expressions.
            // Note access to 2 outer variables.
            // del will be invoked within this method.
            del = () => { j = 10; return j > input; };

            // del2 will be invoked after TestMethod goes out of scope.
            del2 = (x) => { return x == j; };

            // Demonstrate value of j:
            // Output: j = 0 
            // The delegate has not been invoked yet.
            Console.WriteLine("j = {0}", j);

            // Invoke the delegate.
            bool boolResult = del();

            // Output: j = 10 b = True //注意j在del的执行过程中被修改
            Console.WriteLine("j = {0}. b = {1}", j, boolResult);
        }


        static void Main(string[] args)
        {

            //Lambda式可以被赋值给一个委托类型：
            dele myDelegate = x => x * x;
            //例子中的Lambda式中并没有任何类型的声明。是编译器为我们做了相应的隐式数据类型转换：输入参数类型能够从委托的输入参数类型隐式转换，返回类型能够被隐式转换为委托的返回类型。
            int j = myDelegate(5); //j = 25
            Console.WriteLine("j:{0}",j);

            //也可以被用于创建一个表达式树类型：
            Expression<dele> exp = x => x * x;

            //操作符“=>”具有和“=”一样的运算优先级，且为右相关（右边先执行）。

            Program p = new Program();
            p.TestMethod(5);

            // Prove that del2 still has a copy of
            // local variable j from TestMethod. //j的引用超出了原先定义的作用域
            bool result = p.del2(10);


            // Output: True
            Console.WriteLine(result);


            Console.ReadKey();

        }
    }
}
