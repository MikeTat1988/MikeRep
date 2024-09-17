using ShapeLibrary;

namespace hrd27
{
    [TestClass]
    public class UnitTest1
    {
        [TestMethod]
        public void TestCircleArea()
        {
            var circle = new Circle(1.0);
            double area = circle.Area();
            Assert.AreEqual(Math.PI, area, 1E-15);  // 1E-15 is the precision of the comparison
        }

        [TestMethod]
        public void TestRectangleArea()
        {
            var rectangle = new Rectangle(1.0, 2.0);
            double area = rectangle.Area();
            Assert.AreEqual(2.0, area, 1E-15);
        }

        [TestMethod]
        public void TestComplexShapeArea()
        {
            var circle = new Circle(1.0);
            var rectangle = new Rectangle(1.0, 2.0);
            var complexShape = new ComplexShape();
            complexShape.Add(circle);
            complexShape.Add(rectangle);
            double area = complexShape.Area();
            Assert.AreEqual(Math.PI + 2.0, area, 1E-15);
        }
    }
}