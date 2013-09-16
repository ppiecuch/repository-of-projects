using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace _18String
{
    class Program
    {
        public static string toHex(byte[] ba)
        {
            StringBuilder hex = new StringBuilder(ba.Length * 2);
            foreach (byte b in ba)
                hex.AppendFormat("{0:x2}", b);
            return hex.ToString();
        }

        static void Main(string[] args)
        {

            //byte[] data = UTF8Encoding.Default.GetBytes("hello你好");//68656c6c6fc4e3bac3
            //byte[] data = Encoding.Unicode.GetBytes("hello你好"); //680065006c006c006f00604f7d59
            byte[] data = Encoding.UTF8.GetBytes("hello你好");//68656c6c6fe4bda0e5a5bd
            System.Console.WriteLine(toHex(data));

            System.Console.ReadKey();
        }
    }
}
