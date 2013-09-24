using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

//refer to:http://www.cppblog.com/mzty/archive/2006/03/14/4131.html

namespace _25Equals
{
    //使用operator==
    /*public class Base
    {
        protected int x;

        public Base(int x)
        {
            this.x = x;
        }

        public static bool operator ==(Base l, Base r)
        {
            System.Console.WriteLine("operator ==(Base l, Base r)");
            if (object.ReferenceEquals(l, r))
                return true;
            else if (object.ReferenceEquals(l, null) ||
                     object.ReferenceEquals(r, null))
                return false;

            return l.x == r.x;;
        }

        public static bool operator !=(Base l, Base r)
        {
            return !(l == r);
        }

        public int X { get { return x; } }

    }

    public class Derived : Base
    {
        int y;

        public Derived(int x, int y)
            : base(x)
        {
            this.y = y;
        }

        public static bool operator ==(Derived l, Derived r)
        {
            System.Console.WriteLine("operator ==(Derived l, Derived r)");
            if (object.ReferenceEquals(l, r))
                return true;
            else if (object.ReferenceEquals(l, null) ||
                     object.ReferenceEquals(r, null))
                return false;

            return (l.y == r.y) && (l.X == r.X);
        }

        public static bool operator !=(Derived l, Derived r)
        {
            return !(l == r);
        }

        public int Y { get { return y; } }

    }*/

    //使用Equals
    public class Base
    {
        protected int x;

        public Base(int x)
        {
            this.x = x;
        }

        public static bool operator ==(Base l, Base r)
        {
            System.Console.WriteLine("operator ==(Base l, Base r)");
            if (object.ReferenceEquals(l, r))
                return true;
            else if (object.ReferenceEquals(l, null) ||
                     object.ReferenceEquals(r, null))
                return false;

            return l.Equals(r);
        }

        public static bool operator !=(Base l, Base r)
        {
            return !(l == r);
        }

        public int X { get { return x; } }


        public override bool Equals(object obj)
        {
            System.Console.WriteLine("Base::Equals");
            if (obj == null)
                return false;

            Base o = obj as Base;

            if (o != null)
                return x == o.x;
            return false;
        }

        public override int GetHashCode()
        {
            return x.GetHashCode();
        }
    }

    public class Derived : Base
    {
        int y;

        public Derived(int x, int y)
            : base(x)
        {
            this.y = y;
        }

        public int Y { get { return y; } }

        public override bool Equals(object obj)
        {
            System.Console.WriteLine("Derived::Equals");
            if (!base.Equals(obj))
                return false;

            Derived o = obj as Derived;

            if (o == null)
                return false;

            return y == o.y;
        }

        public override int GetHashCode()
        {
            return x.GetHashCode() ^ y.GetHashCode() + x;
        }
    }

    class Program
    {
        static void Main(string[] args)
        {
            //使用operator==
            /*
            Derived d1 = new Derived(2, 11);
            Derived d2 = new Derived(2, 11);

            Console.WriteLine(d1 == d2);    //TRUE 调用Derived operator
            Console.ReadLine();

            //because C# (and thus, most other languages) figure out which == operator method to call based on 
            //the known (i.e. compile-time) type of the object on the left hand side of the operation.
            Base d3 = new Derived(2, 11);
            Base d4 = new Derived(2, 12);

            Console.WriteLine(d3 == d4);    //TRUE 调用Base operator
            Console.ReadLine();*/

            Derived d1 = new Derived(2, 11);
            Derived d2 = new Derived(2, 11);

            Console.WriteLine(d1 == d2);    //TRUE 调用Derived Equals

            //because C# (and thus, most other languages) figure out which == operator method to call based on 
            //the known (i.e. compile-time) type of the object on the left hand side of the operation.
            Base d3 = new Derived(2, 11);
            Base d4 = new Derived(2, 12);

            Console.WriteLine(d3 == d4);    //FALSE 调用Derived Equals
            Console.ReadLine();
        }
    }
}
