using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

//refer to:http://www.sufeinet.com/thread-3508-1-1.html
//refer to:http://www.sufeinet.com/thread-3826-1-1.html

/**
下面我列一下他们的基本属性
名称    所在类              取值范围                                        说 明
 
int     System.Int32        -2147483648--2147483647                         这里大家一定要注意，很容易就会超出
 
string  System.String       Unicode字符串                                   一般是说无限大，大家基本不用担心
 
long    System.Int64        -9223372036854775808-922337206854775807         其实真的不小了
 
byte    System.Byte         0-255                                           这个在网络传输时最常用
 
boolean System.Boolean      NA 表示一个真假值
 
float   System.Single       -1.5乘以10的-45次方  到  3.4乘以10的38次方      一般用于有小数时使用
 
double  System.Double       -5.0乘以10的-324次方  到  1.7乘以10的308次方    一般用于有小数时使用 

decimal System.Decimal      -1.0乘以10的--28次方  到  7.9乘以10的28次方     一般用于金额时使用 

char    System.Char         表示一个16位的Unicode字符                       这个表示一个单个字符

在字符串String中的转义列表

转义序列    表示字符 

\'          单引号 ‘
\"          双引号 “ 
\\          反斜杠 \ 
\0          空 
\a          警告 
\b          退格 
\f          换页 
\n          换行 
\r          回车 
\t          水平制表符 
\v          垂直制表我符 

*/

//类型转换的分类
//C#有两种转换数据类型的方式：隐式转换方式和显式转换方式
/**
可以隐式转换类型
源类型  目的类型 
sbyte   short,int,long,float,double,decimal 
byte    short,ushort,int,uint,long,ulong,float,double,decimal 
short   int,long,float,double,decimal 
ushort  int,uint,long,ulong,float,double,decimal 
int     long,float,double,decimal 
uint    long,ulong,float,double,decimal 
long,   ulong float,double,decimal 
float   double 
char    ushort,int,uint,long,ulong,float,double,decimal 
*/
//显式转换方式
//显式转换，也可以叫做强制转换，因为在显式转换过程中是很有可能出错的。





namespace _03BasicType
{
    class Program
    {
        public enum Mode
        {
            Console = 1,
            Storage = 2
        }

        private static int modeMask = 0;

        public static void setMode(Mode m, bool on)
        {
            modeMask = ((int)modeMask & (~(int)m)) | ((on?-1:0) & (int)m);
        }

        public static bool getMode(Mode m){
			return ((int)modeMask&(int)m)!=0;
		}

        static void Main(string[] args)
        {

            Console.WriteLine("modeMask:" + modeMask + "," + getMode(Mode.Console)+","+getMode(Mode.Storage));
            setMode(Mode.Console, true);
            Console.WriteLine("modeMask:" + modeMask + "," + getMode(Mode.Console) + "," + getMode(Mode.Storage));
            setMode(Mode.Storage, true);
            Console.WriteLine("modeMask:" + modeMask + "," + getMode(Mode.Console) + "," + getMode(Mode.Storage));
            setMode(Mode.Console, false);
            Console.WriteLine("modeMask:" + modeMask + "," + getMode(Mode.Console) + "," + getMode(Mode.Storage));
            setMode(Mode.Storage, false);
            Console.WriteLine("modeMask:" + modeMask + "," + getMode(Mode.Console) + "," + getMode(Mode.Storage));

            //统计
            int count = 0;
            //字符串
            string s = "sufei";
            //是否存在
            Boolean x = true;
            //检查s里面是否有su字符串的出现
            if (s.Contains("su"))
            {
                x = true;
                count = count + 1;
                //或者a++;
            }
            else
            {
                x = false;
            }

            Console.WriteLine("x={0};count={1}", x, count);


            try
            {
                long b = 3000000000;
                int a = checked((int)b);//checked关键字可以检查数据类型的转换是否安全
            }
            catch (Exception e)
            {

                //throw;//算术运算导致溢出。
                Console.WriteLine(e.ToString());
            }

            try
            {
                string c = "105.5";
                int d = int.Parse(c);
            }
            catch (System.Exception e)
            {
                //throw;//输入字符串的格式不正确。
                Console.WriteLine(e.ToString());
            }

            //所有的Object类型基本上都有一个toString()方法，这是系统重载过的
            string g = "105";
            int h = Convert.ToInt32(g);

            string f = "2013-05-01";
            DateTime dt = Convert.ToDateTime(f);
            Console.WriteLine("{0},{1}",h,dt);


            Console.ReadKey();
        }
    }
}
