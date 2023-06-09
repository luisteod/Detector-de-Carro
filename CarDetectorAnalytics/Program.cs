using System;
using System.IO.Ports;
using System.IO;


namespace CarDetectorAnalytics
{
    internal class Program
    {
        static SerialPort serialPort;
        string port; 
        public Program(string[] args)
        {
            serialPort = new SerialPort(args[0], Int32.Parse(args[1]));
        }
        static int Main(string[] args)
        {
            if (args.Length == 0)
            {
                Console.WriteLine("Program closed, please specify PORT and BAUDRATE of the device");
                return 0;
            }
            new Program(args);
            byte[] Data = new byte[1024];

            while (true)
            {

                serialPort.Open();
                serialPort.Read(Data, 0, 1024);
            }
        }
    }
}
