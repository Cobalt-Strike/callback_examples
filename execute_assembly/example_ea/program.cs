using System;

namespace bexecute_assembly 
{
    internal class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("Execute Assembly callback example:");

            // Write out the arguments
            foreach (string arg in args)
            {
                Console.WriteLine("d:{0}", arg);

                // Wait before sending the next item.
                System.Threading.Thread.Sleep(5000);
            }

            Console.WriteLine("done");
        }
    }
}
