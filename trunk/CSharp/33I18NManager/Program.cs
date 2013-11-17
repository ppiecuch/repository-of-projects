using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace _33I18NManager
{
    class Program
    {
        static void Main(string[] args)
        {
            System.Console.WriteLine("I18N:"+I18NManager.getInstance().getString(1,'a',2));
            System.Console.ReadKey();
        }
    }
}
