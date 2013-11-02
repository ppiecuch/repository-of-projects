using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

//C#的设计缺陷（2）：不能以void作为泛型参数
//http://star.baidu.com/forum/forum.php?mod=viewthread&tid=407

//The following kinds of type cannot be used as arguments in instantiations (of generic types or methods):
//Byref types (e.g., System.Generic.Collection.List`1<string&> is invalid)
//Value types that contain fields that can point into the CIL evaluation stack (e.g.,List<System.RuntimeArgumentHandle>)
//void (e.g., List<System.Void> is invalid)

//refer to:http://www.ibcibc.com/thread-1066-1-1.html

//泛型是在.net 2.0中出现的，并且作为基本语法集成在CLR中，所以不仅C#中存在泛型，包括VB.NET，visual C++.NET都可以使用泛型。.NET中的泛型和C++中的模板是类似的，至少在语法表现上。

//为什么要使用泛型？
//在.NET 2.0之前并没有泛型这个概念。当时如果需要动态存储数据的话，只能是把把所有的数据都放在一个ArrayList里面。
//而ArrayList会将所有类型（值类型，引用类型）的变量都装箱为object之后再存储，读取的时候，还需要将这些数据进行拆箱操作。
//不仅影响了性能，而且还容易出现运行时错误。而泛型就是为了弥补这一点，在类，方法等的定义是使用占位符来声明数据的类型，比如说定义了一个List<int>的变量，
//则这个变量只能接受int类型的值，而不能接受string类型的值。这样既保证了性能(不用拆箱和装箱)，而且还是类型安全(Type Safe)的。


//泛型类型名称命名规则
//上面提到了占位符，理论上讲除了关键字之外都是可以用来当做站位符的。不过微软给出了一些建议：
//1.泛型类型名称应以T开头；
//2.如果该泛型类型没有任何约束，则可以简单的使用T来表示， 例如 Add<T>(T t1, T t2){ ... }；
//3.如果该泛型存在约束，比如继承，或实现接口等，则尽量使用明确的名称，
//比如 Add<TDocument>() where TDocument : IDocument

//泛型类
//泛型类是比较常见的，比如我们常用的List, Dictionary等都是泛型类。

//约束(Constraints)
//使用约束的意义在于能够确保代码的正确性，减少不必要的运行时错误(runtime exception)。
/**
内容                说明 
where T : struct    T必须是值类型 
where T : class     T必须是引用类型 
where T : IFoo      T必须实现接口IFoo 
where T : Foo       T必须继承自Foo 
where T : New()     T必须包含默认构造函数 
where T1 : T2       T1继承自另一个参数T2 
*/


//继承
//当一个类（可以是泛型类，也可以是非泛型类）继承自一个泛型类的时候，有一定的要求：
//当子类是非泛型类的时候，父类(泛型类)没有约束；
//当子类是泛型类的时候，则要求父类和子类的占位符必须是相同的或是父类的占位符是明确的，
//例如 Child<T> : Base<T>或是 Child<T> : Base<string>


//泛型接口
//泛型接口和泛型类类似，在这里不做过多的描述

namespace _08Generics
{

    public class Document<T> where T : class
    {
        public Document()
        {
            //泛型类型的默认值(default value)。
            //在使用反省类型的时候可能会用到占位符T的默认值。在这里不能直接将null赋值给T。因为null只能应用于引用类型(reference type), 而不能应用于值类型(value type)。为了能保证正确赋值，需要使用关键字default。即
            Title = default(T);
            Content = default(T);
            //这样编译器就会根据实际情况自动赋值，比如给引用类型默认值为null，int类型默认值为0等。
        }

        public Document(T title, T content)
        {
            this.Title = title;
            this.Content = content;
        }

        public T Title {get; set;}
        public T Content {get; set;}
    }

    class Program
    {

        //ref是传递参数的地址，out是返回值，两者有一定的相同之处，不过也有不同点。
        //使用ref前必须对变量赋值，out不用。
        //out的函数会清空变量，即使变量已经赋值也不行，退出函数时所有out引用的变量都要赋值，ref引用的可以修改，也可以不修改。

        //泛型方法
        //泛型方法的定义和泛型类是一样的，只不过对象有类变为方法。例如
        //swap有更强悍的应用，参考
        //《C++的营养——swap手法》：http://www.kuqin.com/language/20080227/3994.html
        static void Swap<T>(ref T lhs, ref T rhs)
        {
            T temp;
            temp = lhs;
            lhs = rhs;
            rhs = temp;
        }


        static void Main(string[] args)
        {
            Document<string> doc = new Document<string>();
            doc.Title = "Title1";
            doc.Content = "Content1";
            Console.WriteLine("doc:{0},{1}", doc.Title, doc.Content);


            int a = 10;
            int b = 20;
            Swap<int>(ref a, ref b);
            Console.WriteLine("a:{0},b{1}", a, b);

            Console.ReadKey();
        }
    }
}
