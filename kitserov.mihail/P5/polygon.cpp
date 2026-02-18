#include "polygon.hpp"
#include <stdexcept>
#include "utils.hpp"

kitserov::Polygon::Polygon(point_t* vertices, size_t vertexCount):
  vertices_(new point_t[vertexCount]),
  vertexCount_(vertexCount),
  center_(calculateCentroid(vertices, vertexCount))
{
  if (vertexCount < 3) {
    delete[] vertices_;
    throw std::logic_error("Polygon must have at least 3 vertices");
  }
  for (size_t i = 0; i < vertexCount; ++i) {
    vertices_[i] = vertices[i];
  }
}

kitserov::Polygon::~Polygon()
{
  delete[] vertices_;
}

float kitserov::Polygon::getArea() const noexcept
{
  float area = 0;
  for (size_t i = 0; i < vertexCount_; ++i) {
    size_t j = (i + 1) % vertexCount_;
    area += vertices_[i].x * vertices_[j].y - vertices_[j].x * vertices_[i].y;
  }
  return std::abs(area) / 2.0;
}

kitserov::rectangle_t kitserov::Polygon::getFrameRect() const noexcept
{
  if (vertexCount_ == 0) {
    return {0, 0, {0, 0}};
  }

  float minX = vertices_[0].x;
  float minY = vertices_[0].y;
  float maxX = vertices_[0].x;
  float maxY = vertices_[0].y;

  for (size_t i = 1; i < vertexCount_; ++i) {
    minX = std::min(minX, vertices_[i].x);
    minY = std::min(minY, vertices_[i].y);
    maxX = std::max(maxX, vertices_[i].x);
    maxY = std::max(maxY, vertices_[i].y);
  }

  float width = maxX - minX;
  float height = maxY - minY;
  float centerX = minX + width / 2;
  float centerY = minY + height / 2;

  return {width, height, {centerX, centerY}};
}

void kitserov::Polygon::move(float dx, float dy) noexcept
{
  for (size_t i = 0; i < vertexCount_; ++i) {
    vertices_[i].x += dx;
    vertices_[i].y += dy;
  }
  center_.x += dx;
  center_.y += dy;
}

void kitserov::Polygon::move(const point_t& p) noexcept
{
  float dx = p.x - center_.x;
  float dy = p.y - center_.y;
  move(dx, dy);
}

void kitserov::Polygon::doScale(float k)
{
  for (size_t i = 0; i < vertexCount_; ++i) {
    vertices_[i].x = center_.x + (vertices_[i].x - center_.x) * k;
    vertices_[i].y = center_.y + (vertices_[i].y - center_.y) * k;
  }
}
