using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

//refer to:http://www.ibcibc.com/thread-1078-1-1.html

//封装的意义
//封装的意义在于保护或者防止代码（数据）被我们无意中破坏。在面向对象程序设计中数据被看作是一个中心的原素并且和使用它的函数结合的很密切，从而保护它不被其它的函数意外的修改。
//封装提供了一个有效的途径来保护数据不被意外的破坏。相比我们将数据（用域来实现）在程序中定义为公用的（public）我们将它们(fields)定义为私有的（privat）在很多方面会更好。
//私有的数据可以用两种方式来间接的控制。下面我们看一些c#例子来学习这两种方法用以封装数据。第一种方法，我们使用传统的存、取方法。第二种方法我们用属性（property）。


//refer to:http://tech.ddvip.com/2009-02/1234509362108642.html
//自动属性：
/*
在C# 2.0中，我们对成员进行封装，也就是自动属性，如下示例：
private int m_one; 
public int One { 　　
    get { return m_one; } 　　
    set { m_one = value; } 
} 
这种方式只需写第一句代码，然后用Visual Studio的Refactor->Encapsulate Field功能，很方便，也早已经为大家熟知。
而在C# 3.0中，我们只需要写成如下一句就可以了：
public int One { get; set; }// Auto-implemented properties 
*/

namespace _06Encapsulation
{
    //属性是c#引入的一种语言成分，只有很少的语言支持属性。通过对属性的读和写来保护类中的域。
    public class Department { 
        private string departname;
        public Department Parent{get;set;}
        public string Departname {
            //当然属性也可以是只写的（write-only）/只读（read-only），这只需属性只具有一个set/get操作。
            get { 
                return departname; 
            } 
            set {
                Console.WriteLine("setDepartname:{0}", value);
                departname = value; 
            } 
        } 
    } 

    class Program
    {
        static void Main(string[] args)
        {
            Department d = new Department();
            d.Departname = "Communication";
            Console.WriteLine("The Department is : {0}", d.Departname);

            Department p = new Department();
            p.Departname = "Headquarters";

            d.Parent = p;

            Console.WriteLine("The Department's parent is : {0}", d.Parent.Departname);

            Console.ReadKey();
        }
    }
}
