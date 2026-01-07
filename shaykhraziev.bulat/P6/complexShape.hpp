#ifndef COMPLEXSHAPE_HPP
#define COMPLEXSHAPE_HPP
#include "../common/shape.hpp"
namespace shaykhraziev
{
  struct ComplexShape
  {
    virtual void preappend(Shape* shp) = 0;
    virtual void append(Shape* shp) = 0;
    virtual void add(Shape* shp, size_t index) = 0;
    virtual Shape* last() const = 0;
    virtual Shape* first() const = 0;
    virtual Shape* get(size_t index) const noexcept = 0;
    virtual Shape* at(size_t index) const = 0;
    virtual void remove(size_t index) = 0;
    virtual void dropFirst() noexcept = 0;
    virtual void dropLast() noexcept = 0;
    virtual void clear() noexcept = 0;
    virtual size_t size() const noexcept = 0;
    virtual bool empty() const noexcept = 0;

    virtual ComplexShape() = default;
    virtual ~ComplexShape() = default;

  };
}
#endif