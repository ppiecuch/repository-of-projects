using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

//refer to:http://www.sufeinet.com/thread-3383-1-1.html

//1.C#枚举类型是一种值类型，它用于声明一组命名的常数。
//2.枚举的成员是该枚举类型的命名常数。任意两个枚举成员不能具有相同的名称。每个枚举成员均具有相关联的常数值。此值的类型就是枚举的基础类型。每个枚举成员的常数值必须在该枚举的基础类型的范围之内。
//3.枚举类型能选择的基础类型有：byte,sbyte,shout,ushout,int,uint,long,ulong ，默认的为int

 //访问修辞符 enum 枚举名:基础类型
 //{
 //       枚举成员
 //}
 
namespace _04Enumeration
{

    public enum Color : int
    {
        Red = 1,
        Green = 2,
        Blue = 3
    }

    //如果没有指定值，在枚举类型中声明的第一个枚举成员它的默值为零。以后的枚举成员值是将前一个枚举成员（按照文本顺序）的值加 1 得到的。
    //这样增加后的值必须在该基础类型可表示的值的范围内；否则，会出现编译时错误。 


    class Program
    {
        static void Main(string[] args)
        {
            Color c = Color.Blue;
            string colorName = "";
            switch (c)
            {
            case Color.Red:
                colorName = "Red";
                break;
            case Color.Green:
                colorName = "Green";
                break;
            case Color.Blue:
                colorName = "Blue";
                break;
            default:
                colorName = "";
            }
            Console.WriteLine(colorName + "," + System.Enum.GetNames(typeof(Color)).Length);
            Console.ReadKey();

        }
    }
}
