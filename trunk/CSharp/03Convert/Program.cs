using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

//Convert.ToInt32、int.Parse（Int32.Parse）、int.TryParse、(int) 四者都可以解释为将类型转换为 int，那它们的区别是什么呢？
//Convert.ToInt32 与 int.Parse 较为类似，实际上 Convert.ToInt32 内部调用了 int.Parse：

//Convert.ToInt32 参数为 null 时，返回 0； 
//int.Parse 参数为 null 时，抛出异常。 

//Convert.ToInt32 参数为 "" 时，抛出异常； 
//int.Parse 参数为 "" 时，抛出异常。 

//Convert.ToInt32 可以转换的类型较多； 
//int.Parse 只能转换数字类型的字符串。 
//int.TryParse 与 int.Parse 又较为类似，但它不会产生异常，转换成功返回 true，转换失败返回 false。最后一个参数为输出值，如果转换失败，输出值为 0。

namespace _03Convert
{
    class Program
    {
        static void Main(string[] args)
        {

            try{
                System.Console.WriteLine("Convert.ToInt32:{0}", Convert.ToInt32("-10"));
                System.Console.WriteLine("Convert.ToInt32:{0}", Convert.ToInt32(""));//输入格式字符串不正确
            }catch(Exception e)
            {
                System.Console.WriteLine(e.ToString());
            }

        
            try{
                System.Console.WriteLine("int.Parse:{0}", int.Parse("-10"));
                int result = 0;
                System.Console.WriteLine("int.TryParse:{0}", int.TryParse("", out result));
                System.Console.WriteLine("int.Parse:{0}", int.Parse(""));//输入格式字符串不正确
            }catch(Exception e)
            {
                System.Console.WriteLine(e.ToString());
            }

            System.Console.ReadKey();
        }
    }
}
