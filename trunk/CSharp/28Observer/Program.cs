using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace _28Observer
{
    class Program
    {
        static void Main(string[] args)
        {
            Model m = new Model();
            View1 v1 = new View1(m);
            View2 v2 = new View2(m);

            for (int i = 0; i < 10; ++i)
                m.setValue(i);

            System.Console.Read();
        }

        static string Merge(params Object[] objs)
        {
            string str = "";
            for(int i=0;i<objs.Length;++i)
            {
                str += objs[i] + ",";
            }
            str +="(" + objs.Length + ")";
            return str;
        }

        class Model : Observable{
	
	        private int value;
        	
	        public void setValue(int v){
		        this.value=v;
		        setChanged();
		        notifyObservers(value,1,2,3,4,5,6);
	        }
        	
        }

        class View1 : IObserver{
        	
	        Model model;

	        public View1(Model m){
		        this.model=m;
	            m.addObserver(this);
	        }
        	
	        public void update(Observable o, params Object[] args) {
                System.Console.WriteLine(this + "," + Merge(args));
	        }
        	
        }

        class View2 : IObserver{
        	
	        Model model;

	        public View2(Model m){
		        this.model=m;
	            m.addObserver(this);
	        }

            public void update(Observable o, params Object[] args) {
                System.Console.WriteLine(this + "," + Merge(args));
	        }
        	
        }
    }
}
