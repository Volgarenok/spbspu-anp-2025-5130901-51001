#include <iostream>
#include "rectangle.hpp"
#include "concave.hpp"
#include "triangle.hpp"
#include "functions.hpp"

int main()
{
  using namespace vishnyakov;

  size_t amount_of_shapes = 3;
  Shape *Shapes[amount_of_shapes] = {};

  try
  {
    Shapes[0] = new Rectangle(2.0, 2.0, {0.0, 0.0});
    Shapes[1] = new Triangle({-4.0, -2.0}, {-2.0, 6.0}, {2.0, 0.0});
    Shapes[2] = new Concave({4.0, 3.0}, {-4.0, -2.0}, {5.0, -3.0}, {-1.0, -1.0});

    outputInformation(Shapes, amount_of_shapes);

    double coefficient = 0;
    point_t point = {0.0, 0.0};

    std::cout << "\nEnter point about which shapes will be scale: ";
    if (!(std::cin >> point.x >> point.y))
    {
      std::cerr << "Error: invalid point coordinates\n";

      for (size_t i = 0; i < amount_of_shapes; ++i)
      {
        delete Shapes[i];
      }

      return 1;
    }

    std::cout << "Enter coefficient of scalling: ";
    if (!(std::cin >> coefficient) || coefficient <= 0.0)
    {
      std::cerr << "Error: invalid coefficient\n";

      for (size_t i = 0; i < amount_of_shapes; ++i)
      {
        delete Shapes[i];
      }

      return 1;
    }

    scaleAllShapes(Shapes, amount_of_shapes, point, coefficient);

    std::cout << "\nAfter scalling:\n";

    outputInformation(Shapes, amount_of_shapes);
  }
  catch (const std::bad_alloc&)
  {
    std::cerr << "Memory allocation failed\n";
    for (size_t i = 0; i < amount_of_shapes; ++i)
    {
      delete Shapes[i];
    }
    return 2;
  }

  for (size_t i = 0; i < amount_of_shapes; ++i)
  {
    delete Shapes[i];
  }

  return 0;
}

