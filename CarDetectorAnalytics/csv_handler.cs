using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Reflection;

namespace CarDetectorAnalytics
{

    internal class csv_handler
    {
        public readonly string path = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + "/data.csv";

        public csv_handler()
        {

        }
        public string exists()
        {

            if (!File.Exists(path))
            {
                using (File.Create(path)) { }
            }
            return path;
        }
    }
   
}
