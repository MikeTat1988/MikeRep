using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ShapeLibrary
{
    public class Rectangle : Shape
    {
        public double Width { get; set; }
        public double Length { get; set; }

        static Rectangle()
        {
            Console.WriteLine("Rectangle class initialised!");
        }

        public Rectangle(double width = 1.0, double length = 1.0)
        {
            Width = width;
            Length = length;
        }

        public Rectangle(double width, double length, string color, bool isFilled) : base(color, isFilled)
        {
            Width = width;
            Length = length;
        }

        public override double Area()
        {
            return Width * Length;
        }

        public override double Perimeter()
        {
            return 2 * (Width + Length);
        }

        public override string ToString()
        {
            return $"A Rectangle with width={Width} and length={Length}, which is a Derived class of {base.ToString()}";
        }
    }
}
