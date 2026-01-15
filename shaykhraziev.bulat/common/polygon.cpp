#include "polygon.hpp"
#include <stdexcept>
#include <algorithm>

shaykhraziev::Polygon::Polygon(point_t* a, size_t size) :
  points(a),
  size(size),
  center({0, 0})
{
  if (size == 0) {
    throw std::invalid_argument("Invalid size");
  }

  double x = 0;
  double y = 0;
  double S = 0;

  if (size == 1) {
    x = a[0].x;
    y = a[0].y;
  } else if (size == 2) {
    x = (a[0].x + a[1].x) / 2;
    y = (a[0].y + a[1].y) / 2;
  } else {
    for (size_t i = 0; i < size; i++) {
      size_t j = i + 1;
      size_t k = i - 1;

      if (j == size) {
        j = 0;
      }
      if (i == 0) {
        k = size - 1;
      }

      S += a[i].x * (a[j].y - a[k].y);
      x += (a[i].x + a[j].x) * (a[i].x * a[j].y - a[j].x * a[i].y);
      y += (a[i].y + a[j].y) * (a[i].x * a[j].y - a[j].x * a[i].y);
    }

    S = abs(S) / 2.0;
    x = x / (6 * S);
    y = y / (6 * S);
    area = S;
  }

  center = {x, y};
}

double shaykhraziev::Polygon::getArea() const
{
  return area;
}

shaykhraziev::rectangle_t shaykhraziev::Polygon::getFrameRect() const
{
  double minx = points[0].x;
  double miny = points[0].y;
  double maxx = points[0].x;
  double maxy = points[0].y;

  for (size_t i = 1; i < size; i++) {
    minx = std::min(minx, points[i].x);
    miny = std::min(miny, points[i].y);
    maxx = std::max(maxx, points[i].x);
    maxy = std::max(maxy, points[i].y);
  }

  double width = maxx - minx;
  double height = maxy - miny;
  point_t p = {minx + width / 2, miny + height / 2};

  return {width, height, p};
}

void shaykhraziev::Polygon::move(double dx, double dy)
{
  for (size_t i = 0; i < size; i++) {
    points[i].x += dx;
    points[i].y += dy;
  }
}

void shaykhraziev::Polygon::move(point_t to)
{
  double dx = to.x - center.x;
  double dy = to.y - center.y;
  move(dx, dy);
}

void shaykhraziev::Polygon::scale(double coef)
{
  area *= coef * coef;

  for (size_t i = 0; i < size; ++i) {
    points[i].x = center.x + (points[i].x - center.x) * coef;
    points[i].y = center.y + (points[i].y - center.y) * coef;
  }
}