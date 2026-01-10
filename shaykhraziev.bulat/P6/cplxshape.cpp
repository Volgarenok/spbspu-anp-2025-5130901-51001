#include "cplxshape.hpp"
#include <algorithm>
#include "../common/utils.hpp"

double shaykhraziev::CplxShape::getArea() const
{
  double area = 0.0;
  for (size_t i = 0; i < size(); ++i) {
    area += get(i)->getArea();
  }
  return area;
}

shaykhraziev::rectangle_t shaykhraziev::CplxShape::getFrameRect() const
{
  if (empty()) {
    return {0.0, 0.0, center};
  }
  rectangle_t frame = get(0)->getFrameRect();
  double left = frame.pos.x - frame.width / 2;
  double right = frame.pos.x + frame.width / 2;
  double top = frame.pos.y + frame.height / 2;
  double bottom = frame.pos.y - frame.height / 2;

  for (size_t i = 1; i < size(); ++i) {
    frame = get(i)->getFrameRect();
    left = std::min(left, frame.pos.x - frame.width / 2);
    right = std::max(right, frame.pos.x + frame.width / 2);
    top = std::max(top, frame.pos.y + frame.height / 2);
    bottom = std::min(bottom, frame.pos.y - frame.height / 2);
  }

  double width = right - left;
  double height = top - bottom;
  point_t pos = {left + width / 2, bottom + height / 2};
  return {width, height, pos};
}

void shaykhraziev::CplxShape::move(point_t to)
{
  double dx = to.x - center.x;
  double dy = to.y - center.y;
  for (size_t i = 0; i < size(); ++i) {
    get(i)->move(dx, dy);
  }
  center = to;
}

void shaykhraziev::CplxShape::move(double dx, double dy)
{
  for (size_t i = 0; i < size(); ++i) {
    get(i)->move(dx, dy);
  }
  center.x += dx;
  center.y += dy;
}

void shaykhraziev::CplxShape::scale(double coef)
{
  for (size_t i = 0; i < size(); ++i) {
    scaleRelative(*get(i), center, coef);
  }
}

shaykhraziev::Shape** shaykhraziev::extend(Shape** curr, size_t cap, size_t add)
{
  Shape** newshapes = new Shape*[cap + add];
  for (size_t i = 0; i < cap; ++i) {
    newshapes[i] = curr[i];
  }
  delete[] curr;
  return newshapes;
}

shaykhraziev::Shape** shaykhraziev::addAt(Shape** curr, Shape* shp, size_t& size, size_t& capacity, size_t index)
{
  if (size + 1 > capacity) {
    Shape** extended = extend(curr, capacity, 1);
    if (capacity != 0) delete[] curr;
    curr = extended;
    ++capacity;
  }
  for (size_t i = size; i > index; --i) {
    curr[i] = curr[i - 1];
  }
  curr[index] = shp;
  ++size;
  return curr;
}

void shaykhraziev::CplxShape::preappend(Shape* shp)
{
  shapes = addAt(shapes, shp, shapes_size, shapes_cap, 0);
  center = calcCenter(shapes, shapes_size);
}

void shaykhraziev::CplxShape::append(Shape* shp)
{
  shapes = addAt(shapes, shp, shapes_size, shapes_cap, shapes_size);
  center = calcCenter(shapes, shapes_size);
}

void shaykhraziev::CplxShape::add(Shape* shp, size_t index)
{
  if (index > shapes_size) {
    throw std::out_of_range("Index out of range");
  }
  shapes = addAt(shapes, shp, shapes_size, shapes_cap, index);
  center = calcCenter(shapes, shapes_size);
}

shaykhraziev::Shape* shaykhraziev::CplxShape::last()
{
  if (empty()) {
    throw std::out_of_range("Index out of range");
  }
  return shapes[shapes_size - 1];
}

shaykhraziev::Shape* shaykhraziev::CplxShape::first()
{
  if (empty()) {
    throw std::out_of_range("Index out of range");
  }
  return shapes[0];
}

shaykhraziev::Shape* shaykhraziev::CplxShape::get(size_t index) noexcept
{
  if (index >= shapes_size) {
    return nullptr;
  }
  return shapes[index];
}

shaykhraziev::Shape* shaykhraziev::CplxShape::at(size_t index)
{
  if (index >= shapes_size) {
    throw std::out_of_range("Index out of range");
  }
  return shapes[index];
}

const shaykhraziev::Shape* shaykhraziev::CplxShape::last() const
{
  if (empty()) {
    throw std::out_of_range("Index out of range");
  }
  return shapes[shapes_size - 1];
}

const shaykhraziev::Shape* shaykhraziev::CplxShape::first() const
{
  if (empty()) {
    throw std::out_of_range("Index out of range");
  }
  return shapes[0];
}

const shaykhraziev::Shape* shaykhraziev::CplxShape::get(size_t index) const noexcept
{
  if (index >= shapes_size) {
    return nullptr;
  }
  return shapes[index];
}

const shaykhraziev::Shape* shaykhraziev::CplxShape::at(size_t index) const
{
  if (index >= shapes_size) {
    throw std::out_of_range("Index out of range");
  }
  return shapes[index];
}

shaykhraziev::Shape** shaykhraziev::rmAt(Shape** curr, size_t& size, size_t index)
{
  if (index >= size) {
    throw std::out_of_range("Index out of range");
  }
  for (size_t i = index; i < size; ++i) {
    curr[i] = curr[i + 1];
  }
  --size;
  return curr;
}

void shaykhraziev::CplxShape::remove(size_t index)
{
  shapes = rmAt(shapes, shapes_size, index);
  center = calcCenter(shapes, shapes_size);
}

void shaykhraziev::CplxShape::dropFirst() noexcept
{
  if (!empty()) {
    shapes = rmAt(shapes, shapes_size, 0);
    center = calcCenter(shapes, shapes_size);
  }
}

void shaykhraziev::CplxShape::dropLast() noexcept
{
  if (!empty()) {
    shapes = rmAt(shapes, shapes_size, shapes_size - 1);
    center = calcCenter(shapes, shapes_size);
  }
}

void shaykhraziev::CplxShape::clear() noexcept
{
  shapes_size = 0;
  center = {0.0, 0.0};
}

size_t shaykhraziev::CplxShape::size() const noexcept
{
  return shapes_size;
}

bool shaykhraziev::CplxShape::empty() const noexcept
{
  return shapes_size == 0;
}

void shaykhraziev::CplxShape::reserve(size_t capacity)
{
  if (capacity > shapes_cap) {
    shapes = extend(shapes, shapes_cap, capacity - shapes_cap);
    shapes_cap = capacity;
  }
}

void shaykhraziev::CplxShape::shrink()
{
  if (shapes_size < shapes_cap) {
    Shape** newshapes = new Shape*[shapes_size];
    for (size_t i = 0; i < shapes_size; ++i) {
      newshapes[i] = shapes[i];
    }
    delete[] shapes;
    shapes = newshapes;
    shapes_cap = shapes_size;
  }
}

size_t shaykhraziev::CplxShape::capacity() const noexcept
{
  return shapes_cap;
}

shaykhraziev::CplxShape::~CplxShape()
{
  delete[] shapes;
}

shaykhraziev::CplxShape::CplxShape() { }

shaykhraziev::point_t shaykhraziev::calcCenter(Shape **curr, size_t size)
{
  double x = 0.0;
  double y = 0.0;

  for (size_t i = 0; i < size; ++i) {
    rectangle_t frame = curr[i]->getFrameRect();
    x += frame.pos.x / size;
    y += frame.pos.y / size;
  }

  return {x, y};
}

