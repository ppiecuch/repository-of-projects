using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

//refer to:http://www.cnblogs.com/fumj/archive/2012/03/22/2412441.html

namespace _26Params
{
    class Program
    {
        public delegate bool testBool(params object[] args);

        //在使用不定长参数params时,如果除了params参数还有其他参数,params参数要作为最后一个参数.否则程序无法识别.
        public static void test(int sd, params int[] arr)//不定长参数前要加params
        {
            int len = arr.Length;
            string str = "";
            foreach(int i in arr)
            {
                str += "," + i;
            }
            Console.WriteLine("第一个参数是:{0}------{1}", sd, str);
        }

        public static bool testBool0(){
            Console.WriteLine("testBool0");
            return true;
        }

        public static bool testBool1(object o1)
        {
            object[] os = o1 as object[];
            if(os!=null)
                Console.WriteLine("testBool1 object[]：" + os[0]);
            else
                Console.WriteLine("testBool1：" + o1);
            return true;
        }

        public static bool testBool2(object o1, object o2)
        {
            Console.WriteLine("testBool2");
            return true;
        }

        static void Main(string[] args)
        {
            test(22, 5, 6, 7, 8, 9, 10);

            //compile failed
            //testBool tb0 = testBool0;
            //tb0();

            testBool tb1 = testBool1;
            tb1(1);

            testBool tba = (object[] os)=>{
                Console.WriteLine("tba object[]：" + os[0]);
                return true;
            };
            tba(10);

            //compile failed
            //testBool tb2 = testBool2;
            //tb2(null, null);

            Console.ReadKey();
        }
    }
}
