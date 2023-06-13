using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace CarDetectorAnalytics
{
    
    internal class csv_handler
    {
        readonly string path = Directory.GetCurrentDirectory()+"/data.csv";

        public csv_handler()
        {

        }
        public bool exists()
        {

            if(!File.Exists(path))
            {
                using (File.Create(path)) { }
            }
            return true;
        }
        public bool writeline(string data)
        {
            if (this.exists())
            {
                using (StreamWriter sw = new StreamWriter(path))
                {
                    sw.WriteLine(data);
                }
            }
            return true;
        }
    }
}
