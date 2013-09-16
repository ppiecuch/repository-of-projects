using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

//http://msdn.microsoft.com/en-us/library/0yw3tz5k.aspx

//In versions of C# before 2.0, the only way to declare a delegate was to use named methods. 
//C# 2.0 introduced anonymous methods and in C# 3.0 and later, lambda expressions supersede anonymous methods as the preferred way to write inline code.
//However, the information about anonymous methods in this topic also applies to lambda expressions. There is one case in which an anonymous method provides functionality not found in lambda expressions. 
//Anonymous methods enable you to omit the parameter list. 
//This means that an anonymous method can be converted to delegates with a variety of signatures. This is not possible with lambda expressions. 


//refer to:http://wenku.baidu.com/view/b71ba6a6d1f34693dbef3e04.html

//在C#里面，变量作用域有三种，一种是属于类的，我们常称之为field；
//第二种则属于函数的，我们通常称之为局部变量；还有一种，其实也是属 于函数的，不过它的作用范围更小，它只属于函数局部的代码片段，这种同样称之为局部变量。
//这三种变量的生命周期基本都可以用一句话来说明，每个变量都属于 它所寄存的对象，即变量随着其寄存对象生而生和消亡。
//对应三种作用域我们可以这样说，类里面的变量是随着类的实例化而生，同时伴随着类对象的资源回收而消 亡(当然这里不包括非实例化的static和const对象)。
//而函数(或代码片段)的变量也随着函数(或代码片段)调用开始而生，伴随函数(或代码片 段)调用结束而自动由GC释放，它内部变量生命周期满足先进后出的特性。

//通过委托特别是匿名委托这层对象的包装，我们就可以突破无法将函数当做对象传递的限制了。

//闭包其实就是使用的变量已经脱离其作用域，却由于和作用域存在上下文关系，从而可以在当前环境中继续使用其上文环境中所定义的一种函数对象。



namespace _12Anonymous
{

    // Create a delegate. 
    delegate void Del(int x);

    public class TCloser
    {
        public Func<int> T1()
        {
            var n = 999;
            return () =>{
                Console.WriteLine(n);
                return n;
            };
        }
    }

    //从上面的代码我们不难看到，变量n实际上是属于函数T1的局部变量，它本来生命周期应该是伴随着函数T1的调用结束而被释放掉的，但这里我们却 在返回的委托b中仍然能调用它，这里正是闭包所展示出来的威力。
    //因为T1调用返回的匿名委托的代码片段中我们用到了n，而在编译器看来，这些都是合法的， 因为返回的委托b和函数T1存在上下文关系，也就是说匿名委托b是允许使用它所在的函数或者类里面的局部变量的，
    //于是编译器通过一系列动作(具体动作我们 后面再说)使b中调用的函数T1的局部变量自动闭合，从而使该局部变量满足新的作用范围。

    //因此，如果你看到.NET中的闭包，你就可以像js中那样理解它，由于返回的匿名函数对象是在函数T1中生成的，因此相当于它是属于T1的一个 属性。
    //如果你把T1的对象级别往上提升一个层次就很好理解了，这里就相当于T1是一个类，而返回的匿名对象则是T1的一个属性，对属性而言，它可以调用它 所寄存的对象T1的任何其他属性或者方法，
    //包括T1寄存的对象TCloser内部的其他属性。如果这个匿名函数会被返回给其他对象调用，那么编译器会自动 将匿名函数所用到的方法T1中的局部变量的生命周转期自动提升，并与匿名函数的生命周期相同，这样就称之为闭合。


    public class TCloser2
    {
        public Func<int> T1()
        {
            var n = 999;
            Func<int> result = () =>{
                return n;
            };
            n = 10;
            return result;
        }

        //dynamic需要C#4.0才能支持，VS2008中的使用是C#3.5
        public dynamic T2()
        {
            var n = 999;
            dynamic result = new { A = n };
            n = 10;
            return result;
        }
    }



    class Program
    {
        static void Main(string[] args)
        {
            // Instantiate the delegate using an anonymous method.
            Del d = delegate(int k) {
                Console.WriteLine("Anonymous demo!");
            };

            d(0);

            System.Threading.Thread t1 = new System.Threading.Thread
            (delegate()
            {
                Console.Write("Hello, ");
                Console.WriteLine("World!");
            });
            t1.Start();

            var a = new TCloser();
            var b = a.T1();
            Console.WriteLine(b());

            
            var a2 = new TCloser2();
            var b2 = a2.T1();
            var c2 = a2.T2();
            Console.WriteLine(b2());
            Console.WriteLine(c2.A);

            //最后输出结果是什么呢？答案是10和999，因为闭包的特性，这里匿名函数中所使用的变量就是实际T1中的变量，与之相反的是，匿名对象 result里面的A只是初始化时被赋予了变量n的值，它并不是n，所以后面n改变之后A并未随之而改变。
            //这正是闭包的魔力所在。   
            //你可能会好奇.NET本身并不支持函数对象，那么这样的特性又是从何而来呢？答案是编译器，我们一看IL代码便会明白了。


            //看到这里想必你已经明白了，在C#中，原来闭包只是编译器玩的花招而已，它仍然没有脱离.NET对象生命周期的规则。
            //它将需要修改作用域的变量 直接封装到返回的类中，变成类的一个属性n，从而保证了变量的生命周期不会随函数T1调用结束而结束，因为变量n在这里已经成了返回的类的一个属性了。
            //看到这里我想大家应该大体上了解C#闭包的来龙去脉了吧。

            //C#中，闭包其实和类中其他属性、方法是一样的，它们的原则都是下一层可以畅快的调用 上一层定义的各种设定，但上一层则不具备访问下一层设定的能力。
            //即类中方法里的变量可以自由访问类中的所有属性和方法，而闭包又可以访问它的上一层即方法 中的各种设定。但类不可以访问方法的局部变量，同理，方法也不可以访问其内部定义的匿名函数所定义的局部变量。
            //这正是C#中的闭包，它通过超越Java语言的委托打下了闭包的第一步基础，随后又通过各种语法糖和编译器来实现如今在.NET世界全面开花的Lamda和LINQ，也使得我们能够编写出更加简洁优雅的代码。

            Console.ReadKey();

        }
    }
}
