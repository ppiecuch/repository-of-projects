using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;

namespace _35ProducerAndConsumer
{
    public class Consumer
    {
        //定义一个消费的方法
        public void Consumption(Goods goods)
        {
            //Goods goods = (Goods)this.container[0];

            Console.WriteLine("消费了物品：" + goods.ToString());

            //消费掉容器中的一个物品
            //this.container.RemoveAt(0);
        }
    }
    public class Producer
    {
        ArrayList container = null;
        private static int counter = 0;

        //得到一个容器
        public Producer(ArrayList container)
        {
            this.container = container;
        }

        //定义一个生产物品的方法装入容器
        public void Product()
        {
            string name = counter.ToString();
            counter++;
            //创建一个新物品装入容器
            Goods goods = new Goods();
            goods.Name = name;
            this.container.Add(goods);

            Console.WriteLine("生产了物品：" + goods.ToString());
        }
    }
    public class Goods
    {
        //物品名称
        private string name;

        public string Name
        {
            get { return name; }
            set { name = value; }
        }
        //重写ToString()
        public override string ToString()
        {
            return "物品名称：" + name;
        }
    }
    class Program
    {
        //创建一个消费者和生产者共用的一个容器
        ArrayList container = new ArrayList();

        Producer producer = null;
        Consumer consumer = null;

        static void Main(string[] args)
        {
            Program p = new Program();

            //创建两个线程并启动
            Thread p1 = new Thread(new ThreadStart(p.ThreadProduct));
            Thread p2 = new Thread(new ThreadStart(p.ThreadProduct));
            Thread c = new Thread(new ThreadStart(p.ThreadConsumption));

            p1.Priority = ThreadPriority.Lowest;
            p2.Priority = ThreadPriority.Lowest;
            c.Priority = ThreadPriority.Highest;

            c.Start();
            
            p1.Start();
            p2.Start();

            //Console.Read();

        }
        //定义一个线程方法生产8个物品
        public void ThreadProduct()
        {
            //创建一个生产者
            producer = new Producer(this.container);
            lock (this)
            {
                for (int i = 1; i <= 8; i++)
                {
                    //如果容器中没有就进行生产
                    //if (this.container.Count == 0)
                    {
                        //调用方法进行生产
                        producer.Product();
                        //生产好了一个通知消费者消费
                        Monitor.Pulse(this);
                    }
                    //容器中还有物品等待消费者消费后再生产
                    Monitor.Wait(this);
                }
            }
        }
        //定义一个线程方法消费生产的物品
        public void ThreadConsumption()
        {
            //创建一个消费者
            consumer = new Consumer();
            /*lock (this)
            {
                while (true)
                {
                    //如果容器中有商品就进行消费
                    if (this.container.Count != 0)
                    {
                        //调用方法进行消费
                        consumer.Consumption();
                        Monitor.Pulse(this);
                    }
                    //容器中没有商品通知消费者消费
                    Monitor.Wait(this);
                }
            }*/
            while (true)
            {
                Goods goods = null;
                lock(this){
                    if (this.container.Count > 0)
                    {
                        goods=(Goods)this.container[0];
                        this.container.Remove(goods);
                    }
                }
                if (goods != null)
                    consumer.Consumption(goods);
                else
                {
                    //容器中没有商品通知生产者
                    lock (this)
                    {
                        Monitor.Pulse(this);
                        Monitor.Wait(this);
                    }
                }
            }
        }
    }
}
