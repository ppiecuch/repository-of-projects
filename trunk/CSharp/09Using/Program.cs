using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

//refer to:http://developer.51cto.com/art/201105/263550.htm

//using 语句允许程序员指定使用资源的对象应当何时释放资源。为 using 语句提供的对象必须实现 IDisposable 接口。此接口提供了 Dispose 方法，该方法将释放此对象的资源。

//using的多重身份： 
//1、引入命名空间 
//2、创建别名 
//3、强制资源清理 


//创建命名空间别名 
//using为命名空间创建别名的用法规则为：
//using alias = namespace | type;  其中namespace表示创建命名空间的别名；而type表示创建类型别名。\
//例如，在.NET Office应用中，常常会引入Microsoft.Office.Interop.Word.dll程序集，在引入命名空间时为了避免繁琐的类型输入，我们通常为其创建别名如下：
//using MSWord = Microsoft.Office.Interop.Word;  这样，就可以在程序中以MSWord来代替Microsoft.Office.Interop.Word前缀


//强制资源清理 
//用途：清理非托管资源，不受GC控制的资源。Using结束后会隐式的调用Disposable方法。
//用法:
//using (Class1 c = new Class1())   
//{
//}//清理非托管不受GC控制的资源  


//但是当对象使用using关键字的时候，必须实现IDisposable接口。其实using的使用和try-catch-finaly中，在finaly代码域调用Disposable方法效果一样。注意Using不能使用多个不同的类。
//Class1 f = new Class1();   
//try
//{ 
//  执行代码   
//}
//catch()
//{
//  异常处理
//}
//finally
//{
//  f.Disposable();
//} 
//using 语句获取一个或多个资源，执行一个语句，然后处置该资源。

//using 语句： using ( 资源获取 ) 嵌入语句
//资源获取： 局部变量声明
//表达式
//资源是实现 System.IDisposable 的类或结构，它包含名为 Dispose 的单个无参数方法。
//正在使用资源的代码可以调用 Dispose 以指示不再需要该资源。
//如果不调用 Dispose，则最终将因为垃圾回收而发生自动处置。

//如果资源获取的形式是局部变量声明，那么此局部变量声明的类型必须为System.IDisposable 或是可以隐式转换为 System.IDisposable 的类型。
//如果资源获取的形式是表达式，那么此表达式必须是 System.IDisposable 或是可以隐式转换为 System.IDisposable 的类型。


//在资源获取中声明的局部变量必须是只读的，且必须包含一个初始值设定项。
//using 语句被翻译成三个部分：获取、使用和处置。资源的使用隐式封闭在包含一个 finally 子句的 try语句中。
//此 finally 子句处置资源。如果获取了 null 资源，则不进行对 Dispose 的调用，也不引发任何异常。


//例如，下列形式的 using 语句
//using (R r1 = new R()) {
//  r1.F();  
//}  
//完全等效于
//R r1 = new R();   
//try {   
//  r1.F();   
//}   
//finally {   
//if (r1 != null) 
//  ((IDisposable)r1).Dispose();   
//} 

//using语句允许程序员指定使用资源的对象应当何时释放资源。为using语句提供的对象必须实现IDisposable接口。此接口提供了Dispose方法，该方法将释放此对象的资源。
//可以在到达using语句的末尾时，或者在该语句结束之前引发了异常并且控制权离开语句块时，退出using语句。
//可以在using语句中声明对象（如上所示），或者在using语句之前声明对象，如下所示：
//Font font2 = new Font("Arial", 10.0f);
//using (font2){
// use font2
//} 



namespace _09Using
{

    public class MyObject : IDisposable 
    { 

        public MyObject() 
        { 
        } 

        public void Dispose ( ) 
        { 
            // Dispose 
            Console.WriteLine ( "Disposed" ) ; 
        } 

    } 


    class Program
    {
        static void Main(string[] args)
        {

            using (MyObject o1 = new MyObject())
            {
                Console.WriteLine("quit1");
            }

            MyObject o2 = new MyObject();
            using(o2)
            {
                Console.WriteLine("quit2");
            }

            Console.WriteLine("End");
            Console.ReadKey();

        }
    }
}
