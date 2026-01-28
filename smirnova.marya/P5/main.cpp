#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <cstddef>

#include "inputData.hpp"
#include "frameRect.hpp"
#include "rectangle.hpp"
#include "shape.hpp"
#include "square.hpp"
#include "triangle.hpp"
#include "utils.hpp"

int main()
{
  using namespace smirnova;

  Rectangle rect(4.0, 2.0, {0.0, 0.0});
  Square sq(3.0, {5.0, 5.0});
  Triangle tri({0.0, 0.0}, {4.0, 0.0}, {2.0, 3.0});
  const size_t sizeShapes = 3;
  Shape* shapes[sizeShapes] = {&rect, &tri, &sq};
  const char* names[sizeShapes] = {"Rectangle", "Triangle", "Square"};
  std::cout << "Before scaling:\n";
  inputData(shapes, names, sizeShapes);
  point_t scalePoint;
  double k = 0.0;
  std::cout << "\nEnter the zoom point (x y): ";
  if (!(std::cin >> scalePoint.x >> scalePoint.y))
  {
    std::cerr << "Input error: Invalid scaling point\n";
    return 1;
  }
  std::cout << "Enter the scaling factor: ";
  if (!(std::cin >> k) || k <= 0.0)
  {
    std::cerr << "Input error: Invalid scaling factor\n";
    return 1;
  }
  for (size_t i = 0; i < sizeShapes; ++i)
  {
    rectangle_t frame = shapes[i]->getFrameRect();
    double dx = scalePoint.x - frame.pos.x;
    double dy = scalePoint.y - frame.pos.y;
    shapes[i]->move(dx, dy);
    shapes[i]->scale(k);
    shapes[i]->move(-dx, -dy);
  }
  std::cout << "\nAfter scaling:\n";
  inputData(shapes, names, sizeShapes);
  return 0;
}

