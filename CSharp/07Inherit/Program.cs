using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

//refer to:http://www.cnblogs.com/jiajiayuan/archive/2011/09/09/2172292.html
//refer to:http://www.cnblogs.com/michaelxu/archive/2007/03/29/692021.html


//派生类隐式获得基类的除构造函数和析构函数以外的所有成员。
//派生类只能有一个直接基类，所以C#并不支持多重继承，但一个基类可以有多个直接派生类。
//继承是可以传递的。


//如果只有派生类定义构造函数时，只需构造派生类对象即可。对象的基类部分使用默认构造函数来自动创建。
//当基类和派生类都定义有构造函数时，那么执行顺序会怎样呢？
//如果基类中是没有参数的构造函数，那么他可以隐式的被派生类执行，也就是说，派生类根本不需要包含构造函数
//如果基类中是没有参数的构造函数，在派生类中可以自定义有参数的构造函数

//如果派生类的基类也是派生类，则每个派生类只需负责其直接基类的构造，不负责间接基类的构造，
//并且其执行构造函数的顺序是从最上面的基类开始的，直到最后一个派生类结束。


//使用C#接口应注意几个问题：
//1、C#中的接口是独立于类来定义的。这与 C++模型是对立的，在 C++中接口实际上就是抽象基类。
//2、接口和类都可以继承多个接口。
//3、类可以继承一个基类，接口根本不能继承类。这种模型避免了 C++的多继承问题，C++中不同基类中的实现可能出现冲突。
//因此也不再需要诸如虚拟继承和显式作用域这类复杂机制。C#的简化接口模型有助于加快应用程序的开发。
//4、一个接口定义一个只有抽象成员的引用类型。C#中一个接口实际所做的，仅仅只存在着方法标志，但根本就没有执行代码。这就暗示了不能实例化一个接口，只能实例化一个派生自该接口的对象。
//5、接口可以定义方法、属性和索引。所以，对比一个类，接口的特殊性是：当定义一个类时，可以派生自多重接口，而你只能可以从仅有的一个类派生


/*
    重写和隐藏的定义:
    重写：基类方法声明为virtual(虚方法)，派生类中使用override申明此方法的重写.
    隐藏：基类方法不做申明（默认为非虚方法），在派生类中使用new声明此方法的隐藏。
*/

//refer to:http://www.cnblogs.com/blsong/archive/2010/08/12/1798064.html
//abstract:声明抽象类、抽象方法
//1.抽象方法所在类必须为抽象类
//2.抽象类不能直接实例化，必须由其派生类实现。
//3.抽象方法不包含方法主体，必须由派生类以override方式实现此方法,这点跟interface中的方法类似

//virtual:标记方法为虚方法
//1.可在派生类中以override覆盖此方法
//2.不覆盖也可由对象调用
//3.无此标记的方法(也无其他标记)，重写时需用new隐藏原方法(个人理解：没有new也会被默认new，除非使用了override)

namespace _07Inherit
{
    //能够阻止某个类被其他类继承吗？
    //答案是可以的，C#提供了一个sealed 修饰符，此修饰符会阻止其他类从该类继承。
    class A
    {
        public void Sum(int i, int j)
        {
            int sum = i + j;
            Console.WriteLine("I am A ,my sum ={0}", sum);
        }
    }

    //声明接口在语法上和声明抽象类完全相同，例如这里有一个银行账户的接口：
    public interface IBankAccount
    {
        void PayIn(decimal amount);
        bool Withdraw(decimal amount);

        decimal Balance
        {
            get;
        }   
    }

    //派生类隐式获得基类的除构造函数和析构函数以外的所有成员。
    class B : A, IBankAccount
    {

        private decimal balance;

        public decimal Balance
        {
            get
            {
                return balance;
            }
        }

        public void PayIn(decimal amount)
        {
            balance += amount;
        }

        public bool Withdraw(decimal amount)
        {
            if (balance >= amount)
            {
                balance -= amount;
                return true;
            }
            Console.WriteLine("Withdraw failed.");
            return false;
        }

        public override string ToString()
        {
            return String.Format("Venus Bank Saver:Balance={0,6:C}", balance);
        }


        public void Minus(int i, int j)
        {
            int minus = i - j;
            Console.WriteLine("I am B ,my minus ={0}", minus);

            this.Sum(3, 4);
        }

    }

    class AA
    {
        public virtual void ClassAA() //有无virtual都一样
        {
            Console.WriteLine("AA.ClassAA()");
        }

        //public virtual void protectedMethod(){}
        //protected virtual void protectedMethod() { }
    }
    class BB : AA
    {
        new public void ClassAA()       //相当于重载，没有new时被默认为重载
        {
            Console.WriteLine("BB.ClassAA()");
        }

        //错误 当重写“protected”继承成员“_07Inherit.AA.protectedMethod()”时，无法更改访问修饰符
        //protected override void protectedMethod() { }
        //错误 当重写“protected”继承成员“_07Inherit.AA.protectedMethod()”时，无法更改访问修饰符
        //public override void protectedMethod() { }

    }

    //
    class AA2
    {
        public virtual void ClassAA2()//必须有virtual才可以被覆盖
        {
            Console.WriteLine("AA2.ClassAA2()");
        }
    }
    class BB2 : AA2
    {
        public override void ClassAA2() //相当于覆盖
        {
            Console.WriteLine("BB2.ClassAA2()");
        }
    }

    abstract class Base{
        //public abstract void test() {}//编译错误：无法声明主体，因为它标记为 abstract
        //public abstract virtual void test();///编译错误：抽象方法“_07Inherit.Base.test()”不能标记为 virtual
        public abstract void test();
    }

    abstract class Derived : Base{
        public abstract void demo();
    }

    //abstract class Super : Derived//也可以将之声明为abstract class，此时无法new，编译错误：无法创建抽象类或接口“_07Inherit.Super”的实例
    class Super : Derived
    {   
        public override void test(){
            Console.WriteLine("test");
        }
        public override void demo()
        {
            Console.WriteLine("demo");
        }
    }

 

    class Program
    {
        static void Main(string[] args)
        {

            Super s = new Super();
            s.test();
            s.demo();

            B b = new B();
            b.Minus(3, 4);

            b.PayIn(10.5M);

            Console.WriteLine("End:{0}", b.ToString());

            BB bb = new BB();
            bb.ClassAA();//BB.ClassAA();
            AA aa = bb;
            aa.ClassAA();//AA.ClassAA();

            Console.WriteLine("\n");

            BB2 bb2 = new BB2();
            bb2.ClassAA2();//BB.ClassAA2();
            AA2 aa2 = bb2;
            aa2.ClassAA2();//BB.ClassAA2();


            Console.Read();
        }
    }
}
