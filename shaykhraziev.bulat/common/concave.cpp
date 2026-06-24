#include "concave.hpp"
#include <stdexcept>
#include "utils.hpp"
#include <cmath>

shaykhraziev::Concave::Concave(const point_t* a, size_t size) :
  points(),
  center()
{
  if (size != 4) {
    throw std::invalid_argument("Must have four vertices");
  }

  // 0 - 2
  double d1 = distToLine(a[0], a[2], a[1]);
  double d3 = distToLine(a[0], a[2], a[3]);

  // 1 - 3
  double d0 = distToLine(a[1], a[3], a[0]);
  double d2 = distToLine(a[1], a[3], a[2]);

  if ((d1 == 0.0 && d3 == 0.0) || round(d1 / abs(d1)) == round(d3 / abs(d3))) {
    if (abs(d1) < abs(d3)) {
      center = a[1];
    } else {
      center = a[3];
    }
  } else if ((d0 == 0.0 && d2 == 0.0) || round(d0 / abs(d0)) == round(d2 / abs(d2))) {
    if (abs(d0) < abs(d2)) {
      center = a[0];
    } else {
      center = a[2];
    }
  } else {
    throw std::invalid_argument("No such concave");
  }

  for (size_t i = 0; i < size; i++) {
    points[i] = a[i];
  }
}

double shaykhraziev::Concave::getArea() const
{
  double d1 = euclidDist(points[0], points[2]);
  double d2 = euclidDist(points[1], points[3]);

  double xx = (points[0].x - points[2].x) * (points[1].x - points[3].x);
  double yy = (points[0].y - points[2].y) * (points[1].y - points[3].y);

  double cosa = abs(xx + yy) / (d1 * d2);
  double sina = sqrt(1 - cosa * cosa);

  return 0.5 * d1 * d2 * sina;
}

shaykhraziev::rectangle_t shaykhraziev::Concave::getFrameRect() const
{
  double minx = points[0].x;
  double miny = points[0].y;
  double maxx = points[0].x;
  double maxy = points[0].y;

  for (size_t i = 1; i < 4; i++) {
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

void shaykhraziev::Concave::move(double dx, double dy)
{
  for (size_t i = 0; i < 4; i++) {
    points[i].x += dx;
    points[i].y += dy;
  }

  center.x += dx;
  center.y += dy;
}

void shaykhraziev::Concave::move(point_t to)
{
  double dx = to.x - center.x;
  double dy = to.y - center.y;
  move(dx, dy);
}

void shaykhraziev::Concave::scale(double coef)
{
  for (size_t i = 0; i < 4; i++) {
    points[i].x = center.x + (points[i].x - center.x) * coef;
    points[i].y = center.y + (points[i].y - center.y) * coef;
  }
}