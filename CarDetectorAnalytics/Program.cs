using System;
using System.IO.Ports;
using System.IO;
using System.Text;
using System.Threading;
using System.Windows;

namespace CarDetectorAnalytics
{
    internal class Program
    {
        readonly string port = "COM4";
        readonly string baud = "115200";

        static SerialPort serialPort;
        private csv_handler csv;
        public Program()
        {
            serialPort = new SerialPort(port, Int32.Parse(baud));
            csv = new csv_handler();
        }
        static int Main()
        {
            Program p = new Program();

            string Data = null;

            serialPort.Open();
            serialPort.DiscardOutBuffer();
            serialPort.ReadTimeout = 50000;
            using (StreamWriter sw = new StreamWriter(p.csv.path))
            {
                while (true)
                {
                    try
                    {
                        Data = serialPort.ReadLine();
                    }
                    catch (Exception ex)
                    {
                        Console.WriteLine(ex.Message);
                        return 0;
                    }
                    if (Data != null)
                    {
                        Data = Data.TrimEnd();
                        Data = Data.Replace(' ', ',');

                        //DataCsv = string.Format("{0},{1}\n", DataArray[0], DataArray[1]);
                        sw.WriteLine(Data);
                        Console.WriteLine(Data);

                        Data = null;

                    }

                }

            }
            return 0;
        }


    }
}
