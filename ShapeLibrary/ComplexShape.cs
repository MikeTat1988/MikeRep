using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ShapeLibrary
{
    public class ComplexShape : Shape
    {
        private List<Shape> shapes;

        public ComplexShape()
        {
            shapes = new List<Shape>();
        }

        public void Add(Shape shape)
        {
            shapes.Add(shape);
        }

        public override double Area()
        {
            return shapes.Sum(s => s.Area());
        }

        public override double Perimeter()
        {
            return shapes.Sum(s => s.Perimeter());
        }

        public override string ToString()
        {
            return $"A ComplexShape composed of {shapes.Count} shapes.";
        }
    }
}
