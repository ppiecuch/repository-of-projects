using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

//refer to:http://www.sufeinet.com/thread-2153-1-1.html

//委托是一个类，它定义了方法的类型，使得可以将方法当作另一个方法的参数来进行传递，这种将方法动态地赋给参数的做法，可以避免在程序中大量使用If-Else(Switch)语句，同时使得程序具有更好的可扩展性。

namespace _10Delegate
{

    public delegate void GreetingDelegate(string name);

    public delegate void ResultOperationHandler<T>(T obj);

    public delegate object ReturnObjectHandler();
    public delegate void ReturnVoidtHandler();

    class Program
    {

        private static void EnglishGreeting(string name)
        {
            Console.WriteLine("Morning, " + name);
        }

        private static void ChineseGreeting(string name)
        {
            Console.WriteLine("早上好, " + name);
        }

        //注意此方法，它接受一个GreetingDelegate类型的方法作为参数
        private static void GreetPeople(string name, GreetingDelegate MakeGreeting)
        {
            MakeGreeting(name);
        }


        // 热水器
        public class Heater
        {
            private int temperature;
            public delegate void BoilHandler(int param);   //声明委托
            public event BoilHandler BoilEvent;        //声明事件


            // 烧水
            public void BoilWater()
            {
                for (int i = 0; i <= 100; i++)
                {
                    temperature = i;
                    if (temperature > 95)
                    {
                        if (BoilEvent != null)
                        { //如果有对象注册
                            BoilEvent(temperature);  //调用所有注册对象的方法
                        }
                    }

                }
            }
        }

        // 警报器
        public class Alarm
        {
            public void MakeAlert(int param)
            {
                Console.WriteLine("Alarm：嘀嘀嘀，水已经 {0} 度了：", param);
            }
        }

        // 显示器
        public class Display
        {
            public static void ShowMsg(int param)
            {
                Console.WriteLine("Display：水已烧开，当前温度：{0}度。", param);
            }
        }

        //Observer设计模式中主要包括如下两类对象：
        //Subject：监视对象，它往往包含着其他对象所感兴趣的内容。在本范例中，热水器就是一个监视对象，它包含的其他对象所感兴趣的内容，就是temprature字段，当这个字段的值快到100时，会不断把数据发给监视它的对象。
        //Observer：监视者，它监视Subject，当Subject中的某件事发生的时候，会告知Observer，而Observer则会采取相应的行动。在本范例中，Observer有警报器和显示器，它们采取的行动分别是发出警报和显示水温。

        //在本例中，事情发生的顺序应该是这样的：

        //警报器和显示器告诉热水器，它对它的温度比较感兴趣(注册)。
        //热水器知道后保留对警报器和显示器的引用。
        //热水器进行烧水这一动作，当水温超过95度时，通过对警报器和显示器的引用，自动调用警报器的MakeAlert()方法、显示器的ShowMsg()方法。

        //类似这样的例子是很多的，GOF对它进行了抽象，称为Observer设计模式：Observer设计模式是为了定义对象间的一种一对多的依赖关系，以便于当一个对象的状态改变时，其他依赖于它的对象会被自动告知并更新。
        //Observer模式是一种松耦合的设计模式。


        static void getVoid(){}
        static object getObject() { return null; }

        public class TestReturn{
            public ReturnObjectHandler roh;
            public ReturnVoidtHandler rvh;

            public TestReturn(ReturnObjectHandler roh){
                this.roh = roh;
            }

            public TestReturn(ReturnVoidtHandler rvh)
            {
                this.rvh = rvh;
            }

        }


        static void Main(string[] args)
        {
            GreetPeople("sufei", EnglishGreeting);
            GreetPeople("苏飞", ChineseGreeting);

            GreetingDelegate delegate1;
            //注意这里，第一次用的“=”，是赋值的语法；第二次，用的是“+=”，是绑定的语法。如果第一次就使用“+=”，将出现“使用了未赋值的局部变量”的编译错误。
            delegate1 = EnglishGreeting; // 先给委托类型的变量赋值
            delegate1 += ChineseGreeting;   // 给此委托变量再绑定一个方法


            GreetPeople("micheal", delegate1);

            delegate1 -= EnglishGreeting; //取消对EnglishGreeting方法的绑定

            GreetPeople("tom", delegate1);

            Heater heater = new Heater();
            Alarm alarm = new Alarm();

            heater.BoilEvent += alarm.MakeAlert;            //注册方法
            heater.BoilEvent += (new Alarm()).MakeAlert;    //给匿名对象注册方法
            heater.BoilEvent += Display.ShowMsg;            //注册静态方法

            heater.BoilWater();   //烧水，会自动调用注册过对象的方法

            //ReturnObjectHandler rh1 = getObject;
            //错误 返回类型错误
            //ReturnObjectHandler rh2 = getVoid;

            //错误	在以下方法或属性之间的调用不明确:“_10Delegate.Program.TestReturn.TestReturn(_10Delegate.ReturnObjectHandler)”和“_10Delegate.Program.TestReturn.TestReturn(_10Delegate.ReturnVoidtHandler)”
            //TestReturn rr1 = new TestReturn(getObject);
            //TestReturn rr2 = new TestReturn(getVoid);

            Console.ReadKey();

        }
    }
}
