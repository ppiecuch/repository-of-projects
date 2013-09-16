using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
//因为在Cs项目 中默认的是不加载using System.Web;这个命名空间的。
//所以我们要手动的添加一个引用才可以
//接着再声明使用此命名空间
using System.Web;

using b = Blogs;    //这种方法是将命名空间起个别名

//refer to:http://www.sufeinet.com/thread-4367-1-1.html
//1.命名空间的定义是有一个规范的。
//2.命名空间的导入是与添加引用息息相关的。
//3.命名空间之间的类或者是对象是相对独立的。
//4.命名空间是可以用别名的。
namespace _02Namespace
{
    class Program
    {
        static void Main(string[] args)
        {
            b.BlogClass c = new b.BlogClass();
            Console.WriteLine(c.GetBlog());
            Console.ReadKey();
        }

        public static string URLDecode(string text)
        {
            return HttpUtility.UrlDecode(text, Encoding.Default);
        }
    }
}
