#include <iostream>
#include "shape.hpp"
#include "rectangle.hpp"
#include "xquare.hpp"
#include "polygon.hpp"
#include "utils.hpp"

int main()
{
  using namespace kitserov;
  const size_t count = 3;
  Shape* shapes[count] = {};
  const char* shapeNames[] = {"Rectangle", "Xquare", "Polygon"};

  try {
    shapes[0] = new Rectangle{{0.0, 0.0}, 1.0, 2.0};
    shapes[1] = new Xquare{{1.0, 0.0}, 2.0};
    point_t triangleVertices[3] = {{3.0, 0.0}, {4.0, 2.0}, {5.0, 0.0}};
    shapes[2] = new Polygon{triangleVertices, 3};
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << "\n";
    for (size_t i = 0; i < count; i++) {
      delete shapes[i];
    }
    return 2;
  }

  std::cout << "BEFORE SCALING\n";
  printAllInfo(std::cout, shapes, shapeNames, count);

  std::cout << "Point for scale:\n";
  float xx = 0.0;
  float yy = 0.0;
  std::cout << "x = ";
  std::cin >> xx;
  std::cout << "y = ";
  std::cin >> yy;
  float k = 0;
  std::cout << "Ratio: ";
  std::cin >> k;
  if (!std::cin) {
    std::cerr << "incorrect input\n";
    for (size_t i = 0; i < count; i++) {
      delete shapes[i];
    }
    return 1;
  }
  for (size_t i = 0; i < count; i++) {
    try {
      scalePoint(shapes[i], {xx, yy}, k);
    } catch (const std::exception& e) {
      std::cerr << "Error: " << e.what() << "\n";
      for (size_t i = 0; i < count; i++) {
        delete shapes[i];
      }
      return 2;
    }
  }
  std::cout << "AFTER SCALING\n";
  printAllInfo(std::cout, shapes, shapeNames, count);

  for (size_t i = 0; i < count; i++) {
    delete shapes[i];
  }
  return 0;
}
