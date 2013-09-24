using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace _21Type
{
    class A{

    }

    class B{

    }

    class Program
    {
        static void Main(string[] args)
        {
            //Type 是抽象类, typeof(类名称) 返回的是继承自Type 的RuntimeType
            //typeof(类名或类实例)，返回类名类引用。（因为所以类型都是从Type类中继承的，所以类名也是类）
            Type t1 = typeof(A);
            Type t2 = typeof(B);

            System.Console.WriteLine(t1.ToString());
            System.Console.WriteLine(t2.ToString());

            System.Console.ReadKey();
        }
    }
}
