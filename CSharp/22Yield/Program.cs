using System;
using System.Collections;
using System.IO;
using System.Threading;


//在使用foreach语句时，需要在迭代的类中实现IEnumerable接口或 ICollection接口。这样做需要一个处理过程，从而变得有点麻烦。
//在C#中，引入迭代器这个概念后，就很好地解决了上述问题。只要在C#项目程序的类或结构中实现迭代器，那么这个类和结构就支持foreach迭代，
//而不再需要实现全部的IEnumerable接口。当编译器检测到迭代器时，项目程序将自动实现IEnumerable接口或实现IEnumerable接口的方法和属性。

/*
迭代器代码块和普通代码块类似，只是迭代器代码块里面存在不定量的
yield
语句。具体使用格式如下：
 
foreach
语句
 
表达式
 
{ 
     yield return 
成员
; 
     
…..
 
     yield break; 
} 
其中，
yield 
return
语句生成迭代的下一个值；
yield 
break
语句设置迭代终止
完成。即关键字
yield
用于设置返回值，当程序达到
yield return
语句时，会
保存当前的位置。当下次调用迭代器时，将从这个位置重新开始执行。
*/

//refer to:http://wenku.baidu.com/view/65ac081cfad6195f312ba65e.html

namespace _22Yield
{
    public class Persons : System.Collections.IEnumerable 
    { 
        #region IEnumerable 
        public System.Collections.IEnumerator GetEnumerator() 
        { 
            yield return "1"; 
            Thread.Sleep(2000); 
            yield return "2";
            Thread.Sleep(2000);
            yield return "3";
            Thread.Sleep(2000); 
            yield return "4";
            Thread.Sleep(2000); 
            yield return "5";
            Thread.Sleep(2000); 
            yield return "6"; 
        } 
        #endregion 
    } 
    class program 
    { 
        static void Main() 
        { 
            Persons arrPersons = new Persons(); 
            foreach (string s in arrPersons) 
            { 
                System.Console.WriteLine(s); 
            } 
            System.Console.ReadKey(); 
        } 
    }


}
