using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;

namespace _32Clock
{
    class Program
    {
        static List<Clock> clocks = new List<Clock>();

        static ulong getMilliTime(){
            DateTime dt = DateTime.Now;
            //Console.WriteLine(dt.Ticks / 10000);
            return (ulong)dt.Ticks / 10000;
        }

        static string getNowTime(){
            //return DateTime.Now.ToString() + " " + DateTime.Now.Millisecond.ToString();
            return DateTime.Now.ToString("HH:mm:ss:fff");
        }

        static void run()
        {
            for(int i=0;i<20000;i++)
            {
                bool result = true;
                foreach (Clock c in clocks)
                    result=c.update(getMilliTime())&&result;
                if (result)
                    break;
                Thread.Sleep(1000);
            }
        }
        static void Main(string[] args)
        {
            //测试一：使用后自身气血增加200点
            Clock c1 = new Clock((Clock c, Clock.Stage s, uint counter) =>
            {
                Console.WriteLine("Stage;" + s + ",自身气血增加200点(Counter:" + counter + "):" + getNowTime());
            });
            Console.WriteLine("c1 start:" + getNowTime());
            c1.start(getMilliTime());
            clocks.Add(c1);

            //测试二：使用后自身气血上限提高20%,持续5秒
            Clock c2 = new Clock(5000, 0, 0, 0)
            .setSetup((Clock c, Clock.Stage s, uint counter) =>
            {
                Console.WriteLine("Stage;" + s + ",装上：自身气血上限提高20%,持续5秒(Counter:" + counter + "):" + getNowTime());
            })
            .setFinish((Clock c, Clock.Stage s, uint counter) =>
            {
                Console.WriteLine("Stage;" + s + ",卸下：自身气血上限提高20%,持续5秒(Counter:" + counter + "):" + getNowTime());
            });
            Console.WriteLine("c2 start:" + getNowTime());
            c2.start(getMilliTime());
            clocks.Add(c2);

            //测试三：使用5秒后，自身每秒恢复气血100点,持续4秒
            Clock c3 = new Clock(4000, 5000, 1000, 0)
            .setTick((Clock c, Clock.Stage s, uint counter) =>
            {
                Console.WriteLine("Stage;" + s + ",触发：使用5秒后，自身每秒恢复气血100点,持续4秒(Counter:" + counter + ")["+ c.getRemainingTime(getMilliTime())+"]" + getNowTime());
            })
            .setFinish((Clock c, Clock.Stage s, uint counter) =>
            {
                Console.WriteLine("Stage;" + s + ",卸下：使用5秒后，自身每秒恢复气血100点,持续4秒(Counter:" + counter + ")[" + c.getRemainingTime(getMilliTime()) + "]" + getNowTime());
            });
            Console.WriteLine("c3 start:" + getNowTime());
            c3.start(getMilliTime());
            clocks.Add(c3);

            //测试四：有一定机率令目标眩晕，每3秒最多触发一次，持续20秒
            Clock c4 = new Clock(20000, 0, 0, 0, 15, 3000)
            .setTick((Clock c, Clock.Stage s, uint counter) =>
            {
                Console.WriteLine("Stage;" + s + ",触发：有一定机率令目标眩晕，每3秒最多触发一次，持续20秒(Counter:" + counter + ")[" + c.getRemainingTime(getMilliTime()) + "]" + getNowTime());
            })
            .setFinish((Clock c, Clock.Stage s, uint counter) =>
            {
                Console.WriteLine("Stage;" + s + ",卸下：有一定机率令目标眩晕，每3秒最多触发一次，持续20秒(Counter:" + counter + ")[" + c.getRemainingTime(getMilliTime()) + "]" + getNowTime());
            });
            Console.WriteLine("c4 start:" + getNowTime());
            c4.start(getMilliTime());
            clocks.Add(c4);

            //测试五：20%的机率回复2000生命，无时间限制，最多可以生效3次
            Clock c5 = new Clock(0, 0, 0, 3, 20, 0)
            .setTick((Clock c, Clock.Stage s, uint counter) =>
            {
                Console.WriteLine("Stage;" + s + ",触发：20%的机率回复2000生命，无时间限制，最多可以生效3次(Counter:" + counter + ")[" + c.getRemainingTime(getMilliTime()) + "]" + getNowTime());
            })
            .setFinish((Clock c, Clock.Stage s, uint counter) =>
            {
                Console.WriteLine("Stage;" + s + ",卸下：20%的机率回复2000生命，无时间限制，最多可以生效3次(Counter:" + counter + ")[" + c.getRemainingTime(getMilliTime()) + "]" + getNowTime());
            });
            Console.WriteLine("c5 start:" + getNowTime());
            c5.start(getMilliTime());
            clocks.Add(c5);

            //测试六：10秒内回复生命600点，受攻击打断效果。
            Clock c6 = new Clock(10000, 0, 1000, 0)
            .setTick((Clock c, Clock.Stage s, uint counter) =>
            {
                Console.WriteLine("Stage;" + s + ",触发：10秒内回复生命600点，受攻击打断效果。(Counter:" + counter + ")[" + c.getRemainingTime(getMilliTime()) + "]" + getNowTime());
            })
            .setFinish((Clock c, Clock.Stage s, uint counter) =>
            {
                Console.WriteLine("Stage;" + s + ",卸下：10秒内回复生命600点，受攻击打断效果。(Counter:" + counter + ")[" + c.getRemainingTime(getMilliTime()) + "]" + getNowTime());
            });
            Console.WriteLine("c6 start:" + getNowTime());
            c6.start(getMilliTime());
            clocks.Add(c6);

            Thread t1 = new Thread(run);
            t1.Start();

            Thread t2 = new Thread(() => {
                Thread.Sleep(5000);
                c6.finish();
            });
            t2.Start();

            Console.ReadKey();
        }
    }
}
