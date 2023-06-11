using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ShapeLibrary
{
    public abstract class Shape
    {
        public string Color { get; set; }
        public bool IsFilled { get; set; }

        public Shape()
        {
            Color = "green";
            IsFilled = true;
        }

        public Shape(string color, bool isFilled)
        {
            Color = color;
            IsFilled = isFilled;
        }

        public abstract double Area();
        public abstract double Perimeter();

        public override string ToString()
        {
            return $"A Shape with colour of {Color} and {(IsFilled ? "" : "not ")}filled";
        }
    }
}
