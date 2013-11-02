using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace _31Predicate
{
    class Program
    {

        private static uint id = 0;

        bool testTrue(){
            return true;
        }

        bool IsGreaterThan50(int i)
        {
            if (i > 50)
                return true;
            else
                return false;
        }

        bool test2(float a, float b)
        {
            return a > b;
        }

        public static uint getId(){
            return id++;
        }

        static void Main(string[] args)
        {
            Program p = new Program();

            List<int> lstInt = new List<int>();
            lstInt.Add(40);
            lstInt.Add(80);
            lstInt.Add(90);

            Predicate<int> pred = p.IsGreaterThan50;
            int i = lstInt.Find(pred);                         // 找到匹配的第一个元素，此处为80    
            Console.WriteLine("大于50的第一个元素为{0}", i);

            List<int> all = lstInt.FindAll(pred);
            for (int j = 0; j < all.Count(); j++)
            {
                Console.WriteLine("大于50的数组中元素为{0}", all[j]);  // 找出所有匹配条件的    
            }

            List<Condition> conditions = new List<Condition>();
            Condition0 c0 = new Condition0(p.testTrue);
            Condition1<int> c1 = new Condition1<int>(p.IsGreaterThan50, 40);
            Condition1<int> c1a = new Condition1<int>(p.IsGreaterThan50, ()=>{return 60;});
            Condition2<float, float> c2 = new Condition2<float, float>(p.test2, () => { return 1; }, () => { return 2; });
            Condition2<float, float> c2a = new Condition2<float, float>(p.test2, 0.1f, 0.01f);

            conditions.Add(c0);
            conditions.Add(c1);
            conditions.Add(c1a);
            conditions.Add(c2);
            conditions.Add(c2a);

            //failed
            //List<Operation<object>> list = new List<Operation<object>>();
            //list.Add(new Operation0<void>(()=>{}));

            Operation op1 = new Operation1<uint>((uint id) => { Console.WriteLine("operation:" + id); }, Program.getId);
            op1.operate();
            op1.operate();
            op1.operate();
            op1.operate();

            //failed
            //object outObj = null;
            //Operation op2 = new Operation2<uint,out object>((uint id,out object obj) =>{ Console.WriteLine("operation:" + id); obj = id;}, Program.getId,out outObj);
            //op2.operate();

            foreach (Condition c in conditions)
                Console.WriteLine(c.isTrue());



            Console.ReadLine();
        }    
    }
}
