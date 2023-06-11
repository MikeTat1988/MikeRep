using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ShapeLibrary;

namespace hrd27
{
    internal class Program
    {
        static void Main(string[] args)
        {
            //ThisStack<int> mystack = new ThisStack<int>();

            //LList<int> myList = new LList<int>();

            //myList.Push(10);
            //myList.Push(20);
            //myList.Push(30);

            //foreach (int item in myList)
            //{
            //    Console.WriteLine(item);
            //}

            Shape circ = new Circle(2, "white", true);

            Console.WriteLine(circ.ToString());

        }
    }
}
