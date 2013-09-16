using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

//结构是这样的：
//Dictionary<[key], [value]> 
//他的特点是存入对象是需要与[key]值一一对应的存入该泛型,通过某一个一定的[key]去找到对应的值


//refer to:http://jingyan.baidu.com/article/9989c7460ab872f648ecfeed.html

//要使用Dictionary集合，需要导入C#泛型命名空间
 //System.Collections.Generic（程序集：mscorlib）

//Dictionary的描述
//1、从一组键（Key）到一组值（Value）的映射，每一个添加项都是由一个值及其相关连的键组成
//2、任何键都必须是唯一的
//3、键不能为空引用null（VB中的Nothing），若值为引用类型，则可以为空值
//4、Key和Value可以是任何类型（string，int，custom class 等）

//其它常见属性和方法的说明：
//Comparer：           获取用于确定字典中的键是否相等的 IEqualityComparer。
//Count：                  获取包含在 Dictionary中的键/值对的数目。
//Item：                    获取或设置与指定的键相关联的值。
//Keys：                   获取包含 Dictionary中的键的集合。
//Values：                获取包含 Dictionary中的值的集合。
//Add：                    将指定的键和值添加到字典中。
//Clear：                  从 Dictionary中移除所有的键和值。
//ContainsKey：      确定 Dictionary是否包含指定的键。
//ContainsValue：   确定 Dictionary是否包含特定值。             
//GetEnumerator：  返回循环访问 Dictionary的枚举数。
//GetType：             获取当前实例的 Type。 （从 Object 继承。）
//Remove：             从 Dictionary中移除所指定的键的值。
//ToString：             返回表示当前 Object的 String。 （从 Object 继承。）
//TryGetValue：      获取与指定的键相关联的值。


namespace _14Dictionary
{
    class Program
    {
        static void Main(string[] args)
        {

            //实例化对象
            Dictionary<int, string> dic = new Dictionary<int, string>(); 
            //对象打点添加
            dic.Add(1, "one"); 
            dic.Add(2, "two"); 
            dic.Add(3, "three"); 
            //提取元素的方法
            string a = dic[1]; 
            string b = dic[2]; 
            string c = dic[3];

            Console.WriteLine("{0},{1},{2}", a, b, c);

            //通过Key查找元素
            if(dic.ContainsKey(1))
            {
                Console.WriteLine("Key:{0},Value:{1}","1", dic[1]);
            }


            //第一种遍历方式
            var result = from pair in dic orderby pair.Key select pair;
            foreach (KeyValuePair<int, string> pair in result)
            {
                Console.WriteLine("Key:{0}, Value:{1}", pair.Key, pair.Value);
            }

            Console.WriteLine("------------------------------------------");

            //第二种遍历方式
            foreach (KeyValuePair<int, string> item in dic)
            {
                Console.WriteLine("key:{0}, value:{1}", item.Key, item.Value);
            }

            Console.WriteLine("------------------------------------------");

            //仅遍历键 Keys 属性
            Dictionary<int,string>.KeyCollection keys=dic.Keys;
            foreach(int key in keys)
            {
                Console.WriteLine("Key = {0}", key);
            }

            dic.Remove(2);

            //仅遍历值 Valus属性
            Dictionary<int, string>.ValueCollection values = dic.Values;
            foreach (string value in values)
            {
                Console.WriteLine("Value = {0}", value);
            }


            Console.ReadKey();

        }
    }
}
