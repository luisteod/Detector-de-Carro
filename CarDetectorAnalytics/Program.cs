using System;
using System.IO.Ports;
using System.IO;
using System.Text;
using System.Threading;

namespace CarDetectorAnalytics
{
    internal class Program
    {
        readonly string port = "COM4";
        readonly string baud = "115200";
        
        static SerialPort serialPort;
         
        public Program()
        {
            serialPort = new SerialPort(port, Int32.Parse(baud));
        }
        static int Main()
        {
            new Program();
            csv_handler csv = new csv_handler();
            string Data;

            serialPort.Open();
            while (serialPort.IsOpen)
            {
                Data = serialPort.ReadLine();
                csv.writeline(Data);
                Console.WriteLine(Data);
            }
            return 0;
        }
    }
}
