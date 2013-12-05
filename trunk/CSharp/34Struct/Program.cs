using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;


namespace _34Struct
{
    interface Able{
        void test();
    }
    //错误	修饰符“abstract”对该项无效
    //abstract struct AbstractStruct{
    //}

    //错误	修饰符“abstract”对该项无效
    //struct AbstractStruct{
    //    public abstract void test(){}
    //}
    struct DemoStruct1 : Able
    {
        public string str;
        public int x;

        //错误:结构不能包含显式的无参数构造函数
        /*public DemoStruct1()
        {
            this.x = 99;
            Console.WriteLine("default");
        }*/

        //错误:只有类类型才能包含析构函数
        //~DemoStruct1(){}

        //声明有参构造函数的时候，要初始化所有的字段
        //错误:在控制返回调用方之前，字段“_34Struct.DemoStruct1.str”必须被完全赋值
        /*public DemoStruct1(int x){
            this.x = x;
            Console.WriteLine("x");
        }*/

        public DemoStruct1(int x, string s){
            this.x = x;
            this.str = s;
            Console.WriteLine("x,s");
        }

        public void test()
        {
            Console.WriteLine("test:"+x);
        }
    }
    

    class Program
    {
        static DemoStruct1 sds1;
        public static void demo()
        {
            DemoStruct1 ds1;

            //C# 中的局部变量经初始化后方可使用。因此，如果像下面这样声明了一个局部变量而未将其初始化：
            //错误:使用了未赋值的局部变量“ds1”
            //ds1.test();

            //成员变量可以直接使用
            sds1.test();

            //错误:使用了未赋值的局部变量“ds1”
            //Console.WriteLine(ds1.x);

            //错误:使用了未赋值的局部变量“ds1”
            //DemoStruct1 ds1a = ds1;

            //参数化构造函数有意确保存在有效的状态，但在创建结构数组时不执行该构造函数。(应该是执行了编译自动添加的“默认构造函数”)
            DemoStruct1[] dss = new DemoStruct1[1];
            Console.WriteLine(dss[0].x);
        }

        static void Main(string[] args)
        {
            demo();
            

            Console.ReadKey();
        }
    }
}
