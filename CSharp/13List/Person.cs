using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace _13List
{
    //泛型集合的排序
    //排序基于比较，要排序，首先要比较。比如有两个数1、2，要对他们排序，首先就要比较这两个数，根据比较结果来排序。
    //如果要比较的是对象，情况就要复杂一点，比如对Person对象进行比较，则既可以按姓名进行比较，也可以按年龄进行比较，这就需要确定比较规则。
    //一个对象可以有多个比较规则，但只能有一个默认规则，默认规则放在定义该对象的类中。默认比较规则在CompareTo方法中定义，该方法属于IComparable<T>泛型接口。

    class Person : IComparable<Person>
    {
        private string name;
        private int age;

        public Person(string name, int age)
        {
            this.name = name;
            this.age = age;
        } 

        public string Name{
            get{
                return name;
            }
            set{
                name = value;
            }
        }

        public int Age{
            get{
                return age;
            }
            set{
                age = value;
            }
        }

        //CompareTo方法的参数为要与之进行比较的另一个同类型对象，返回值为int类型，如果返回值大于0，表示第一个对象大于第二个对象，如果返回值小于0,表示第一个对象小于第二个对象，如果返回0,则两个对象相等。
        
        public int CompareTo(Person p)
        {
            return this.Age - p.Age;
        }

        public int CompareTo(object obj)
        {
            if (obj is Person)
            {
                Person p = obj as Person;
                return CompareTo(p);
            }
            throw new ArgumentException("object is not a Person");
        }

        public bool Equal(Person other)
        {

            return this.Name == other.Name && this.Age == other.Age;

        }


        

    }

    //实际使用中，经常需要对集合按照多种不同规则进行排序，这就需要定义其他比较规则，可以在Compare方法中定义，该方法属于IComparer<T>泛型接口，请看下面的代码：
    class NameComparer : IComparer<Person>
    {
        //存放排序器实例
        public static NameComparer Default = new NameComparer();

        //按姓名比较
        public int Compare(Person p1, Person p2)
        {
            return System.Collections.Comparer.Default.Compare(p1.Name, p2.Name);
        }

        /*
        //Compare方法的参数为要进行比较的两个同类型对象，返回值为int类型，返回值处理规则与CompareTo方法相同。其中的Comparer.Default返回一个内置的Comparer对象，用于比较两个同类型对象。
        public int Compare(Person p1, Person p2)
        {
            //return System.Collections.Comparer.Default.Compare(p1.Name, p2.Name);
            return p1.Age - p2.Age;
        }

        public int Compare(object o1, object o2)
        {
            //is和as的区别
            //关于类型的判断和转换有is和as这2个操作符。
            //具体区别和用法如下is就是处于对类型的判断。返回true和false。如果一个对象是某个类型或是其父类型的话就返回为true，否则的话就会返回为false。另外is操作符永远不会抛出异常。
            //c#便提供了一种新的类型检查，转换方式。即as操作符,他可以在简化代码的同时，提高性能。代码如下：
            //Employee e = o as Employee;
            //if(e != null){   
            //在if语句中使用e}这种as操作即便等同于上面代码，同时只进行了1次的类型检查，所以提高了性能。
            Person p1 = o1 as Person;
            Person p2 = o2 as Person;
            if (p1 == null || p2 == null)
                throw new ArgumentException("object is not a Person");
            return p1.Age - p2.Age;
        }
        */

    }

}
