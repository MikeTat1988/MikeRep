using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ConsoleApp1
{
    internal class Hello
    {
        static void Main(string[] args)
        {
            Console.WriteLine("Hello World!");
            List<int> myList = new List<int> { 1, 2, 3, 4, 5 };
            List<int> evenNumbers = myList.Where(x => x % 2 == 0).ToList();

            foreach (var item in evenNumbers)
            {
                Console.WriteLine(item);
            }

            string myString = null;
            string result = myString ?? "default value";

            Console.WriteLine(myString);

        }
    }
}
