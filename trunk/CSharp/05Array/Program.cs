using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

//http://www.sufeinet.com/thread-436-1-1.html
//C# 数组从零开始建立索引，即数组索引从零开始。C# 中数组的工作方式与在大多数其他流行语言中的工作方式类似。但还有一些差异应引起注意。
//声明数组时，方括号 ([]) 必须跟在类型后面，而不是标识符后面。在 C# 中，将方括号放在标识符后是不合法的语法。

//另一细节是，数组的大小不是其类型的一部分，而在 C 语言中它却是数组类型的一部分。
//这使您可以声明一个数组并向它分配 int 对象的任意数组，而不管数组长度如何。

//C# 支持一维数组、多维数组（矩形数组）和数组的数组（交错的数组）。

namespace _05Array
{
    class Program
    {
        static void Main(string[] args)
        {

            int[] table; // not int table[];

            // declare numbers as an int array of any size
            int[] numbers;
            // numbers is a 10-element array
            numbers = new int[10];
            // now it's a 5-element array
            numbers = new int[5];

            //多维数组：
            string[,] names;
            int[, ,] ages;

            //数组的数组（交错的）：
            byte[][] scores;

            //下面的代码声明了类型为 int 的二维数组的三维数组的一维数组。
            int[][, ,][,] numbers2;

            //声明数组（如上所示）并不实际创建它们。在 C# 中，数组是对象（本教程稍后讨论），必须进行实例化。
            names = new string[5, 4];

            ages = new int[1, 2, 3];

            scores = new byte[5][];
            for (int x = 0; x < scores.Length; x++)
            {
                scores[x] = new byte[4];
            }

            //初始化数组
            //C# 通过将初始值括在大括号 ({}) 内为在声明时初始化数组提供了简单而直接了当的方法。下面的示例展示初始化不同类型的数组的各种方法。
            //注意   如果在声明时没有初始化数组，则数组成员将自动初始化为该数组类型的默认初始值。另外，如果将数组声明为某类型的字段，则当实例化该类型时它将被设置为默认值 null。
            int[] numbers3 = new int[5] { 1, 2, 3, 4, 5 };
            string[] names3 = new string[3] { "Matt", "Joanne", "Robert" };

            //可省略数组的大小，如下所示：
            int[] numbers4 = new int[] { 1, 2, 3, 4, 5 };
            string[] names4 = new string[] { "Matt", "Joanne", "Robert" };

            //如果提供了初始值设定项，则还可以省略 new运算符，如下所示：
            int[] numbers5 = { 1, 2, 3, 4, 5 };
            string[] names5 = { "Matt", "Joanne", "Robert" };

            int[,] numbers6 = new int[3, 2] { { 1, 2 }, { 3, 4 }, { 5, 6 } };
            string[,] siblings6 = new string[2, 2] { { "Mike", "Amy" }, { "Mary", "Albert" } };

            //可省略数组的大小，如下所示：
            int[,] numbers7 = new int[,] { { 1, 2 }, { 3, 4 }, { 5, 6 } };
            string[,] siblings7 = new string[,] { { "Mike", "Amy" }, { "Mary", "Albert" } };

            //如果提供了初始值设定项，则还可以省略 new 运算符，如下所示：
            int[,] numbers8 = { { 1, 2 }, { 3, 4 }, { 5, 6 } };
            string[,] siblings8 = { { "Mike", "Amy" }, { "Mary", "Albert" } };

            int[][] numbers9 = new int[2][] { new int[] { 2, 3, 4 }, new int[] { 5, 6, 7, 8, 9 } };

            //可省略第一个数组的大小，如下所示：
            int[][] numbers10 = new int[][] { new int[] { 2, 3, 4 }, new int[] { 5, 6, 7, 8, 9 } };

            int[][] numbers11 = { new int[] { 2, 3, 4 }, new int[] { 5, 6, 7, 8, 9 } };

            //访问数组成员访问数组成员可以直接进行，类似于在 C/C++ 中访问数组成员。
            numbers[4] = 5;

            //下面的代码声明一个多维数组，并向位于 [1, 1] 的成员赋以 5：
            numbers6[1, 1] = 5;

            numbers11[0][0] = 0;

            //System.Array 是所有数组类型的抽象基类型。可以使用 System.Array 具有的属性以及其他类成员。这种用法的一个示例是使用“长度”(Length)属性获取数组的长度。
            Console.WriteLine("{0},{1}", numbers11.Length, numbers11[0].Length);

            //System.Array 类提供许多有用的其他方法/属性，如用于排序、搜索和复制数组的方法。
            //对数组C# 还提供 foreach 语句。该语句提供一种简单、明了的方法来循环访问数组的元素。
            foreach(int i in numbers)
            {
                Console.WriteLine(i);
            }

            foreach (int i in numbers6)
            {
                Console.Write("{0},", i);
            }

            Console.ReadKey();

        }
    }
}
