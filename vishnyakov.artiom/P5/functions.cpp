#include "functions.hpp"

void vishnyakov::movePoint(point_t &pos, double dx, double dy)
{
  pos.x += dx;
  pos.y += dy;
}

vishnyakov::rectangle_t vishnyakov::getFullFrameRect(const Shape *const *shapes, size_t size)
{
  if (size == 0)
  {
    return rectangle_t{0.0, 0.0, {0.0, 0.0}};
  }

  rectangle_t frame = shapes[0]->getFrameRect();

  double minx = frame.pos.x - frame.width / 2.0;
  double maxx = frame.pos.x + frame.width / 2.0;
  double miny = frame.pos.y - frame.height / 2.0;
  double maxy = frame.pos.y + frame.height / 2.0;

  for (size_t i = 1; i < size; ++i)
  {
    rectangle_t frame = shapes[i]->getFrameRect();

    minx = frame.pos.x - frame.width / 2.0 < minx ? frame.pos.x - frame.width / 2.0 : minx;
    maxx = frame.pos.x + frame.width / 2.0 > maxx ? frame.pos.x + frame.width / 2.0 : maxx;
    miny = frame.pos.y - frame.height / 2.0 < miny ? frame.pos.y - frame.height / 2.0 : miny;
    maxy = frame.pos.y + frame.height / 2.0 > maxy ? frame.pos.y + frame.height / 2.0 : maxy;
  }

  double width = maxx - minx;
  double height = maxy - miny;

  point_t center = {(minx + maxx) / 2.0, (miny + maxy) / 2.0};

  return rectangle_t{width, height, center};
}

void vishnyakov::scaleAllShapes(Shape **shapes, size_t size, point_t point, double coefficient)
{
  for (size_t i = 0; i < size; ++i)
  {
    rectangle_t frame = shapes[i]->getFrameRect();

    double dx = (frame.pos.x - point.x) * (coefficient - 1);
    double dy = (frame.pos.y - point.y) * (coefficient - 1);

    shapes[i]->scale(coefficient);
    shapes[i]->move(dx, dy);
  }
}

void vishnyakov::outputInformation(const Shape *const *shapes, size_t size)
{
  double total_Area = 0.0;

  for (size_t i = 0; i < size; ++i)
  {
    std::cout << "Shape #" << (i+1) << ":\n";
    std::cout << "  Area = " << shapes[i]->getArea() << '\n';
    outputFrameRectInfo(shapes[i]->getFrameRect());
    total_Area += shapes[i]->getArea();
  }

  std::cout << "\nTotal Area = " << total_Area << '\n';

  outputFrameRectInfo(getFullFrameRect(shapes, size));
}

void vishnyakov::outputFrameRectInfo(rectangle_t frame)
{
  std::cout << "  FrameRect: center = {" << frame.pos.x << ", " << frame.pos.y;
  std::cout << "}; width = " << frame.width << "; height = " << frame.height << '\n';
}

