#include "utils.hpp"
#include <algorithm>
#include <cmath>

void shaykhraziev::scaleRelative(Shape& shp, point_t pt, double coef)
{
  if (coef == 1) {
    return;
  }

  point_t c = shp.getFrameRect().pos;
  double x = pt.x + (c.x - pt.x) * coef;
  double y = pt.y + (c.y - pt.y) * coef;

  shp.scale(coef);
  shp.move({x, y});
}

shaykhraziev::rectangle_t shaykhraziev::getAllShapesFrameRect(Shape** shps, size_t size)
{
  if (size == 0) {
    return {0, 0, {0, 0}};
  }

  rectangle_t r = shps[0]->getFrameRect();
  double minx = r.pos.x - r.width / 2;
  double miny = r.pos.y - r.height / 2;
  double maxx = r.pos.x + r.width / 2;
  double maxy = r.pos.y + r.height / 2;

  for (size_t i = 1; i < size; i++) {
    r = shps[i]->getFrameRect();
    minx = std::min(minx, r.pos.x - r.width / 2);
    miny = std::min(miny, r.pos.y - r.height / 2);
    maxx = std::max(maxx, r.pos.x + r.width / 2);
    maxy = std::max(maxy, r.pos.y + r.height / 2);
  }

  double width = maxx - minx;
  double height = maxy - miny;
  point_t c = {minx + width / 2, miny + height / 2};

  return {width, height, c};
}

void shaykhraziev::removeArray(Shape** shps, size_t size)
{
  for (size_t i = 0; i < size; i++) {
    delete shps[i];
  }
}

void shaykhraziev::outputParams(std::ostream& out, Shape** shps, size_t size)
{
  double area = 0;

  for (size_t i = 0; i < size; i++) {
    double x = shps[i]->getArea();
    area += x;
    out << x << "\n";
  }
  out << area << "\n";

  for (size_t i = 0; i < size; i++) {
    rectangle_t r = shps[i]->getFrameRect();
    out << "(" << r.pos.x << ", " << r.pos.y << ") " << r.height << " " << r.width << "\n";
  }

  rectangle_t all = getAllShapesFrameRect(shps, size);
  out << "(" << all.pos.x << ", " << all.pos.y << ") " << all.width << " " << all.height << "\n";
}

double shaykhraziev::euclidDist(point_t d1, point_t d2)
{
  return sqrt((d1.x - d2.x) * (d1.x - d2.x) + (d1.y - d2.y) * (d1.y - d2.y));
}

double shaykhraziev::distToLine(point_t d1, point_t d2, point_t m)
{
  double A = d1.y - d2.y;
  double B = d2.x - d1.x;
  double C = d1.x * d2.y - d1.y * d2.x;

  return (A * m.x + B * m.y + C) / sqrt(A * A + B * B);
}


