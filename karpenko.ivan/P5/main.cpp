#include <iostream>
#include <stdexcept>
#include "shape.hpp"
#include "rectangle.hpp"
#include "triangle.hpp"
#include "complexquad.hpp"
#include "functions.hpp"

int main()
{
  using namespace karpenko;

  const size_t SHAPE_COUNT = 4;
  Shape* shapes[SHAPE_COUNT] = {nullptr};

  try
  {
    shapes[0] = new Rectangle(4.0, 3.0, {0.0, 0.0});
    shapes[1] = new Triangle({-2.0, -1.0}, {2.0, -1.0}, {0.0, 3.0});
    shapes[2] = new ComplexQuad({-1.0, 0.0}, {0.0, -2.0}, {1.0, 0.0}, {0.0, 2.0});
    shapes[3] = new Rectangle(2.0, 5.0, {3.0, 2.0});
  }
  catch (const std::bad_alloc&)
  {
    std::cerr << "Memory allocation failed\n";
    for (size_t i = 0; i < SHAPE_COUNT; ++i)
    {
      delete shapes[i];
    }
    return 1;
  }

  printAllInfo(shapes, SHAPE_COUNT, "Before scaling");

  point_t scalePoint = {0.0, 0.0};
  double coefficient = 1.0;

  std::cout << "\nEnter scaling point (x y): ";
  if (!(std::cin >> scalePoint.x >> scalePoint.y))
  {
    std::cerr << "Error: invalid point coordinates\n";
    for (size_t i = 0; i < SHAPE_COUNT; ++i)
    {
      delete shapes[i];
    }
    return 1;
  }

  std::cout << "Enter scaling coefficient: ";
  if (!(std::cin >> coefficient))
  {
    std::cerr << "Error: invalid coefficient\n";
    for (size_t i = 0; i < SHAPE_COUNT; ++i)
    {
      delete shapes[i];
    }
    return 1;
  }

  try
  {
    scaleShapes(shapes, SHAPE_COUNT, scalePoint, coefficient);
  }
  catch (const std::invalid_argument& e)
  {
    std::cerr << "Error during scaling: " << e.what() << "\n";
    for (size_t i = 0; i < SHAPE_COUNT; ++i)
    {
      delete shapes[i];
    }
    return 1;
  }

  printAllInfo(shapes, SHAPE_COUNT, "After scaling");

  for (size_t i = 0; i < SHAPE_COUNT; ++i)
  {
    delete shapes[i];
  }

  return 0;
}
