using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

using Google;
using Google.ProtocolBuffers;
using study.protobuf;
//using study.protobuf.Proto;

namespace _20Protobuf
{
    class Program
    {

        public static string toHex(byte[] ba)
        {
            StringBuilder hex = new StringBuilder(ba.Length * 2);
            foreach (byte b in ba)
                hex.AppendFormat("{0:x2}", b);
            return hex.ToString();
        }

        static void Main(string[] args)
        {
            Student.Builder builder = new Student.Builder();
            builder.SetId(123456789);
            builder.SetName("xiaoming小明");
            builder.SetAge(10);

            Student stu=builder.Build();
            byte[] data = stu.ToByteArray();

            System.Console.WriteLine(toHex(data));

            Student stt = Student.ParseFrom(data);
            System.Console.WriteLine("name:{0},age:{1},id:{2}",stt.Name,stt.Age,stt.Id);

            System.Console.ReadKey();
        }
    }
}
