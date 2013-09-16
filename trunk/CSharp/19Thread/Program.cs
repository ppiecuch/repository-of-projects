using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;

using System.IO;
using System.Net;
using System.Net.Sockets;

namespace _19Thread
{
    class Program
    {

        static void run()
        {
            try{
                string ip = "192.168.33.69";
                int port = 8081;
                TcpClient client = new TcpClient();
                client.Connect(IPAddress.Parse(ip), port);
                Stream s = client.GetStream();
                byte[] buffer=new byte[11];
                for (int i = 0; i < 10; ++i)
                {
                    System.Console.WriteLine("run:" + i);
                    s.Read(buffer, 0, buffer.Length);
                    //Thread.Sleep(1000);
                }
            }
            catch (ThreadAbortException e)
            {
                System.Console.WriteLine(e.ToString());
            }
        }
        static void Main(string[] args)
        {
            Thread t = new Thread(run);
            t.Start();

            Thread.Sleep(3000);
            t.Abort();

            System.Console.ReadKey();
        }
    }
}
