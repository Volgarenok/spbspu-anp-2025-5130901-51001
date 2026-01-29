#include "utils.hpp"
#include <stdexcept>

kitserov::point_t kitserov::calculateCentroid(const point_t* vertices, size_t count) noexcept
{
  if (vertices == nullptr || count == 0) {
    return {0.0, 0.0};
  }
  float sumX = 0.0;
  float sumY = 0.0;
  for (size_t i = 0; i < count; ++i) {
    sumX += vertices[i].x;
    sumY += vertices[i].y;
  }
  return {sumX / static_cast< float >(count), sumY / static_cast< float >(count)};
}

kitserov::rectangle_t kitserov::frame(const point_t* pts, size_t s) noexcept
{
  float minx = pts[0].x;
  float miny = pts[0].y;
  float maxx = pts[0].x;
  float maxy = pts[0].y;

  for (size_t i = 0; i < s; ++i) {
    minx = std::min(minx, pts[i].x);
    miny = std::min(miny, pts[i].y);
    maxx = std::max(maxx, pts[i].x);
    maxy = std::max(maxy, pts[i].y);
  }

  float width = maxx - minx;
  float height = maxy - miny;
  float centerX = minx + width / 2;
  float centerY = miny + height / 2;

  return rectangle_t{width, height, {centerX, centerY}};
}

void kitserov::frameOutput(std::ostream& os, const rectangle_t& fr) noexcept
{
  float left = fr.pos.x - fr.width / 2;
  float top = fr.pos.y + fr.height / 2;
  float right = fr.pos.x + fr.width / 2;
  float bottom = fr.pos.y - fr.height / 2;
  os << "Frame (leftTop " << left << ", " << top
     << " rightBot " << right << ", " << bottom << ")\n";
}

void kitserov::shapeOutput(std::ostream& os, const Shape* sh, const char* name) noexcept
{
  rectangle_t frameRect = sh->getFrameRect();
  os << name << ": area = " << sh->getArea() << "\n";
  frameOutput(os, frameRect);
}

void kitserov::scalePoint(Shape* sh, const point_t& p, float k)
{
  rectangle_t fr = sh->getFrameRect();
  float dx = p.x - fr.pos.x;
  float dy = p.y - fr.pos.y;
  sh->move(dx, dy);
  sh->scale(k);
  sh->move(-dx * k, -dy * k);
}

kitserov::rectangle_t kitserov::getOverallFrame(const Shape* const* shapes, size_t count) noexcept
{
  rectangle_t firstFrame = shapes[0]->getFrameRect();
  float minX = firstFrame.pos.x - firstFrame.width / 2;
  float maxX = firstFrame.pos.x + firstFrame.width / 2;
  float minY = firstFrame.pos.y - firstFrame.height / 2;
  float maxY = firstFrame.pos.y + firstFrame.height / 2;

  for (size_t i = 1; i < count; ++i) {
    rectangle_t frame = shapes[i]->getFrameRect();
    float left = frame.pos.x - frame.width / 2;
    float right = frame.pos.x + frame.width / 2;
    float bottom = frame.pos.y - frame.height / 2;
    float top = frame.pos.y + frame.height / 2;
    minX = std::min(minX, left);
    maxX = std::max(maxX, right);
    minY = std::min(minY, bottom);
    maxY = std::max(maxY, top);
  }

  float width = maxX - minX;
  float height = maxY - minY;
  float centerX = minX + width / 2;
  float centerY = minY + height / 2;

  return {width, height, {centerX, centerY}};
}

void kitserov::printAllInfo(std::ostream& os, const Shape* const* shapes, const char* const* names, size_t count) noexcept
{
  for (size_t i = 0; i < count; ++i) {
    if (shapes[i] != nullptr) {
      const char* name = (names != nullptr && i < count) ? names[i] : "Shape";
      shapeOutput(os, shapes[i], name);
    }
  }

  float totalArea = 0.0f;
  for (size_t i = 0; i < count; ++i) {
    if (shapes[i] != nullptr) {
      totalArea += shapes[i]->getArea();
    }
  }

  os << "Total area = " << totalArea << "\n";
  rectangle_t overallFrame = getOverallFrame(shapes, count);
  os << "Overall frame:\n";
  frameOutput(os, overallFrame);
}
