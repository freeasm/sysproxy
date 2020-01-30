using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SysproxyTester
{
    public class SysproxyTester
    {
        public static void Main(string[] args)
        {
            Console.WriteLine(NativeMethods.SetDIRECT());
            Console.ReadLine();

            Console.WriteLine(NativeMethods.SetGlobal("127.0.0.1:1080", "<local>"));
            Console.ReadLine();

            Console.WriteLine(NativeMethods.SetURL("http://127.0.0.1:1080/pac"));
            Console.ReadLine();
        }
    }
}
