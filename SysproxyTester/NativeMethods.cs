using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace SysproxyTester
{
    public static class NativeMethods
    {
        [DllImport("..\\..\\..\\..\\sysproxy\\bin\\x64\\Release\\sysproxy", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool SetDIRECT();

        [DllImport("..\\..\\..\\..\\sysproxy\\bin\\x64\\Release\\sysproxy", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool SetGlobal([MarshalAs(UnmanagedType.LPTStr)] string remote, [MarshalAs(UnmanagedType.LPTStr)] string bypass);

        [DllImport("..\\..\\..\\..\\sysproxy\\bin\\x64\\Release\\sysproxy", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool SetURL([MarshalAs(UnmanagedType.LPTStr)] string remote);
    }
}
