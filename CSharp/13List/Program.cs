using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

//refer to:http://www.cnblogs.com/vhtt/archive/2009/12/05/1617835.html

//C#中对集合的全面支持更是该语言的精华之一。
//为什么要用泛型集合？
//在C# 2.0之前，主要可以通过两种方式实现集合：
//1.使用ArrayList
//直接将对象放入ArrayList，操作直观，但由于集合中的项是Object类型，因此每次使用都必须进行繁琐的类型转换。
//2.使用自定义集合类
//比较常见的做法是从CollectionBase抽象类继承一个自定义类，通过对IList对象进行封装实现强类型集合。这种方式要求为每种集合类型写一个相应的自定义类，工作量较大。
//泛型集合的出现较好的解决了上述问题，只需一行代码便能创建指定类型的集合。

//主要利用System.Collections.Generic命名空间下面的List<T>泛型类创建集合



namespace _13List
{
    class Program
    {
        static void Main(string[] args)
        {
            //创建类型为Person的对象集合
            List<Person> persons = new List<Person>();

            //创建Person对象
            Person p1 = new Person("张三", 30);
            Person p2 = new Person("李四", 20);
            Person p3 = new Person("王五", 50);
            Person p4 = new Person("李六", 30);

            //将Person对象放入集合
            persons.Add(p1);
            persons.Add(p2);
            persons.Add(p3);
            persons.Add(p4);

            

            //定义好默认比较规则后，就可以通过不带参数的Sort方法对集合进行排序，如下所示:
            //按照默认规则对集合进行排序
            //refer to:http://www.cnblogs.com/yuliantao/archive/2011/01/23/1942451.html
            //此方法使用类型 T 的默认比较器 Comparer.Default 确定列表元素的顺序。Comparer.Default 属性检查类型 T 是否实现了 IComparable 泛型接口，如果实现了该接口，则使用该实现。
            //否则，Comparer.Default 将检查类型 T 是否实现了 IComparable 接口。如果类型 T 未实现任一接口，则 Comparer.Default 将引发 InvalidOperationException。
            //refer to:http://msdn.microsoft.com/en-us/library/b0zbh7b6.aspx
            //InvalidOperationException
            //The default comparer Comparer<T>.Default cannot find an implementation of the IComparable<T> generic interface or the IComparable interface for type T.
            persons.Sort();
            //或者使用
            //persons.Sort(NameComparer.Default);


            //输出人的姓名
            // Console.Write(persons[1].Name);
            foreach (Person p in persons)
            {
                Console.WriteLine("{0}:{1}", p.Name, p.Age);
            }

            //refer to:http://blog.sina.com.cn/s/blog_5fc9337301011rsn.html

            //查找字符串中包含'李'的字符,利用了匿名方法(第一种方式）    
            List<Person> listFind = persons.FindAll(delegate(Person p)
            {
                return p.Name.Contains("李");
            });

            Console.WriteLine("查找到的结果为：");
            foreach (Person p in listFind)
            {
                Console.WriteLine("{0}:{1}", p.Name, p.Age);
            }

            persons.Remove(p4);

            //第二种方式，这两种方式实际上是等价的 
            List<Person> listFind2 = persons.FindAll(ToFindLi);//传入了一个方法名
            Console.WriteLine("查找到的结果2为：");
            foreach (Person p in listFind2)
            {
                Console.WriteLine("{0}:{1}", p.Name, p.Age);
            }

            //refer to;http://blog.csdn.net/yysyangyangyangshan/article/details/7770783
            Person resultPerson = persons.Find(
                delegate(Person p)
                {
                    return p.Age == 20;
                }
            );

            Console.WriteLine(resultPerson != null ? "找到:"+resultPerson.Name + System.Environment.NewLine + resultPerson.Name : "没有查找到");  


            Console.ReadKey();
        }

        private static bool ToFindLi(Person p)
        {
            return p.Name.Contains("李");
        }
    }
}
