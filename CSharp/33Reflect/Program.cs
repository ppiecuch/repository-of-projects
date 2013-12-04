using System;
using System.Reflection;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace _33Reflect
{
    public class Relfect
    {
        public void reflect(int i)
        {
            Console.WriteLine("reflect:" + i);
        }
    }

    class Program
    {
        
        static void Main(string[] args)
        {
            /*Assembly ass = Assembly.GetCallingAssembly();
            Type[] types=ass.GetTypes();
            foreach (Type tt in types)
                Console.WriteLine(tt);
            Type t = ass.GetType("_33Reflect.Relfect");
            MethodInfo mi = t.GetMethod("reflect");
            object obj = ass.CreateInstance("_33Reflect.Relfect");
            mi.Invoke(obj, new object[] { 10 });*/

            Type t = Type.GetType("_33Reflect.Relfect");
            MethodInfo mi = t.GetMethod("reflect");
            object obj = Activator.CreateInstance(t, new object[] { });
            mi.Invoke(obj, new object[] { 10 });

            Console.Read();
        }
    }
}
