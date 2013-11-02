using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace _01Helloworld
{
    class Program
    {
        static void Main(string[] args)
        {
            Version version = System.Environment.Version;
            Console.WriteLine(version.ToString()); //这个只会获取Runtime的.NET 版本

            Console.WriteLine("Helloworld!");
            //控制台应用程序会在执行完毕后立即终止，如果直接通过IDE运行它们，就无法看到运行的结果。为了解决上例中的这个问题，使用
            Console.ReadKey(); 
        }
    }
}
