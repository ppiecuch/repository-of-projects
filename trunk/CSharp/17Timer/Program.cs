using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Timers;
using System.Collections;// for ArrayList

//refer to:《C#定时器的用法.doc》

/*
关于C#中timer类  在C#里关于定时器类就有3个   
1.定义在System.Windows.Forms里   
2.定义在System.Threading.Timer类里   
3.定义在System.Timers.Timer类里  
System.Windows.Forms.Timer是应用于WinForm中的，他是通过Windows消息机制实现的，类似于VB或Delphi中的Timer控件，内部使用API  SetTimer实现的。
他的主要缺点是计时不精确，而且必须有消息循环，Console  Application(控制台应用程式)无法使用。   
  
System.Timers.Timer和System.Threading.Timer很类似，他们是通过.NET  Thread  Pool实现的，轻量，计时精确，对应用程式、消息没有特别的需要。
System.Timers.Timer还能够应用于WinForm，完全取代上面的Timer控件。他们的缺点是不支持直接的拖放，需要手工编码。 

*/

//refer to:http://blog.csdn.net/xv_ly15/article/details/8477779
/*

(1)c#中的时间类：DateTime,TimeSpan
DateTime：
DateTime提供完整的时间计算，如添加年，添加月，添加天，添加小时等操作，解决了闰年的特殊天数，不同月不同天数的复杂计算问题。也提供了时间之间的操作函数，如常用到的相减。
可以通过年，月，日，时，分，秒去构造对象，也可以通过Tick（Tick以100毫微，即100纳秒秒为一个单位）构造。

举几个常用的函数：
1.DayOfWeek ： 获得DateTime对象的日期处于星期几，例如星期一。
2.Add开头的添加时间操作函数：分别有添加年，月，日，时，分，秒，微秒，tick等。
3.Compare：对比函数，对比两个DateTime时间，如果返回值为负数，表示小于被对比的时间，等于零表示和被对比的时间相同，正数表示大于被对比的时间。
4.DayInMonth：这是一个静态函数，可以直接通过DateTime类调用，传入年和月份作为参数就可以返回指定年指定月的天数。
5.Now：获得本地时间
6.时间之间的操作符号：时间相加减，比较等等。
7.To...String：转换成字符串函数
输出结果例子
ToLongDateString： Tuesday, January 08, 2013
ToLongTimeString： 1:02:03 AM
ToShortDateString： 1/8/2013
ToShortTimeString：1:02 AM

TimeSpan：
用于表示时间间隔
可以通过天，时，分，秒构造（不能用月和年，不同于DateTime，当然，TImeSpan和DateTime用途本来就不一样）。
这里讲TimeSpan的原因是，两个DateTime相减获得的就是一个TimeSpan的时间间隔，TimeSpan有获得总天数，总小时数，总分钟数，总秒数的操作。

常用函数：
1.Days,hours,minutes,seconds,milliseconds ：这些函数用于获得该TimeSpan换算成天时分秒后有多少天，多少小时，多少分，多少秒的计算。
2.TotalDays,totalHours,TotalMinutes,TotalSeconds,TotalMilliseconds ：这些函数用于获得总共的天数，小时，分钟，秒（例如一个1时3分5秒的TimeSpan，它的总分钟数为63分钟，而不是上面的minutes的3分钟）

（2）时间差的计算
有了上面的介绍，直接举个例子吧
DateTime tA = new Time(2013,1,5,10,30,0);
//2013年1月5日，10点30分0秒
DateTime tB = new Time(2012,12,31,0,0,0); //2012年12月31日，0点0分0秒
TImeSpan ts = tA - tB;
// ts.Days + "天" + ts.Hours+ "时" + ts.Minutes + "分" + ts.Seconds + "秒";


其实只要有了时间间隔的总秒数，就可以通过一小时等于3600秒的换算方式计算了，例如3661秒就是1小时1分钟1秒了。难就难在如何去求这个时间间隔总秒数。
在C#中用DateTime和TimeSpan
在AS3中用Date（Date有getTime函数，获得的是毫秒数）
*/

namespace _17Timer
{
    class Program
    {
        static void Main(string[] args)
        {
            System.Timers.Timer aTimer = new System.Timers.Timer();
            aTimer.Elapsed += new ElapsedEventHandler(theout);  //到达时间的时候执行事件；
            // 设置引发时间的时间间隔　此处设置为１秒（1000毫秒） 
            aTimer.Interval = 1000;
            aTimer.AutoReset = true;//设置是执行一次（false）还是一直执行(true)；
            aTimer.Enabled = true; //是否执行System.Timers.Timer.Elapsed事件；

            System.Console.ReadKey();
        }

        public static void theout(object source, System.Timers.ElapsedEventArgs e)
        {
            ArrayList AutoTask = new ArrayList();
            AutoTask.Add("8:30:00");
            AutoTask.Add("9:30:00");
            AutoTask.Add("10:30:00");
            AutoTask.Add("11:58:55");
            AutoTask.Add("11:59:00");

            for (int n = 0; n < AutoTask.Count; n++)
            {
                if (DateTime.Now.ToLongTimeString().Equals(AutoTask[n]))
                {
                    System.Console.WriteLine("现在时间是:" + AutoTask[n]);
                }
            }
        }

    }
}
