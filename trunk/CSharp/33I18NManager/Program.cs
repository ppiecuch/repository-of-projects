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
            I18NManager.getInstance().push(1, "注册成功，您的账号是{0}，密码是{1}，价格是{2:F0}");
            System.Console.WriteLine("I18N:"+I18NManager.getInstance().getString(1,'a',2,25.69));
            System.Console.ReadKey();
        }
    }
}
