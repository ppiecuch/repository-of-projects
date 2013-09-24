using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

//refer to:http://www.sudu.cn/info/html/edu/20071219/119837.html

namespace _24InnerClass
{
    class Program
    {

        
        static void Main(string[] args)
        {

            //在C#中，始终可以用（只要访问控制允许） 
            //TestOuter.A instance = new TestOuter.A(); 
            //来创建一个内部类的实例，这个实例与外部类的任何实例没有任何直接的关系。类似于Java中的静态内部类。 


            //而在C#中，类区分为Nested Class和Not-Nested Class，前者是声明在其他数据类型内部的类。后者是直接定义在某一个命名空间的类。 

            Outer1.Inner1 o1i1 = new Outer1.Inner1();
            o1i1.method();

            Outer2.Inner1 o2i1 = new Outer2.Inner1();
            o2i1.method();

            System.Console.ReadKey();

            
        }
    }

    class Outer1{
        public static void static_method()
        {
            System.Console.WriteLine("Outer1.static_method()");
        }

        public void member_method()
        {

        }

        public class Inner1
        {
            public void method()
            {
                //C#中的内部类能够使用外部类定义的类型和静态方法，但是不能直接使用外部类的实例方法
                static_method();
                //member_method(); //无法通过嵌套类型“_24InnerClass.Program.Inner”来访问外部类型“_24InnerClass.Program”的非静态成员
                System.Console.WriteLine("Outer1.Inner1.method()");
            }
        }

        //非内嵌类只允许使用public和internal的访问控制，而内置类则允许使用所有的五种访问控制符，private, protected , internal protected。
        //内部类也可以访问外部类的所有方法,包括instance方法和private方法，但是需要显式的传递一个外部类的实例。 
        class Inner2
        {
            Outer1 outer;

            public Inner2(Outer1 o)
            {
                this.outer = o;

                
            }

            public void method()
            {
                //C#中的内部类能够使用外部类定义的类型和静态方法，但是不能直接使用外部类的实例方法
                static_method();
                outer.member_method();
            }
        }
    }

    //C#的内部类提供了覆盖的功能，在一个包含了内部类的类的子类中可以用关键字new 来覆盖同名内部类的实现。
    class Outer2 : Outer1
    {
        public static void static_method()
        {
            System.Console.WriteLine("Outer2.static_method()");
        }

        //new class Inner1{     //注意：没有public时，Main访问不到此Inner1，编译自动将Outer2.Inner1定位为Outer1.Inner1
        //public class Inner1{  //经测试，有new与没new 都可以定位到Outer2.Inner1 只要有public
        public new class Inner1{
            public void method()
            {
                //C#中的内部类能够使用外部类定义的类型和静态方法，但是不能直接使用外部类的实例方法
                static_method();
                //member_method(); //无法通过嵌套类型“_24InnerClass.Program.Inner”来访问外部类型“_24InnerClass.Program”的非静态成员
                System.Console.WriteLine("Outer2.Inner1.method()");
            }
        }
    }
}
