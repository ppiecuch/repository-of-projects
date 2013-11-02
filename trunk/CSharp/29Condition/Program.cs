using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace _29Condition
{

    

    public class Condition
    {

        public delegate bool CompareHandler(Condition.Comparator com, params object[] os);

        public enum Type{
            ONE = 0,
            COUNT
        }

        public enum Comparator{
            EQ = 0,
            NE,
            GE,
            GT,
            LE,
            LT
        }

        CompareHandler ch = null;
        object[] args = null;
        Comparator com;

        public bool isTrue() { return ch(com,args); }

        public Condition(CompareHandler ch, Condition.Comparator com,params object[] args)
        {
            this.ch = ch;
            this.com = com;
            this.args = args;
        }
    }

    

    class Program
    {

        public static bool compareMP(Condition.Comparator com, params object[] os)
        {
            Console.WriteLine("compareMP:"+os[0]);

            int mp = Convert.ToInt32(os[0]);
            return mp>=100;
        }

        public static bool compareTP(Condition.Comparator com, params object[] os)
        {
            Console.WriteLine("compareTP:" + os[0] +","+ os[1]);

            int mp = Convert.ToInt32(os[0]);
            int tp = Convert.ToInt32(os[1]);
            return mp >= 100 && tp >= 100;
        }

        static int[] cs=new int[(int)Condition.Type.COUNT];


        static void Main(string[] args)
        {
            Condition.CompareHandler ch = compareMP;
            ch(Condition.Comparator.EQ,0);

            Condition c1 = new Condition(compareMP, Condition.Comparator.EQ, 10);
            Console.WriteLine(c1.isTrue());

            Condition c2 = new Condition(compareMP, Condition.Comparator.EQ, 100);
            Console.WriteLine(c2.isTrue());

            Condition c3 = new Condition(compareTP, Condition.Comparator.EQ, 100, 200);
            Console.WriteLine(c3.isTrue());

            Console.ReadKey();
        }
    }
}
