using System;
using System.IO.Ports;
using System.IO;


namespace CarDetectorAnalytics
{
    internal class Program
    {
        static SerialPort serialPort;
        public Program()
        {
            serialPort = new SerialPort("COM4", 115200);
        }
        static void Main(string[] args)
        {
            new Program();
            byte[] Data = new byte[1024];

            while (true)
            {

                serialPort.Open();
                serialPort.Read(Data, 0, 1024);
            }
        }
    }
}
