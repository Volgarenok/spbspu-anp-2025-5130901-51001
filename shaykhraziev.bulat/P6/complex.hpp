#ifndef COMPLEX_HPP
#define COMPLEX_HPP
#include "../common/shape.hpp"
namespace shaykhraziev
{
  struct Complex
  {
    virtual void preappend(Shape* shp) = 0;
    virtual void append(Shape* shp) = 0;
    virtual void add(Shape* shp, size_t index) = 0;
    virtual Shape* last() = 0;
    virtual Shape* first() = 0;
    virtual Shape* get(size_t index) noexcept = 0;
    virtual Shape* at(size_t index) = 0;
    virtual const Shape* last() const = 0;
    virtual const Shape* first() const = 0;
    virtual const Shape* get(size_t index) const noexcept = 0;
    virtual const Shape* at(size_t index) const = 0;
    virtual void remove(size_t index) = 0;
    virtual void dropFirst() noexcept = 0;
    virtual void dropLast() noexcept = 0;
    virtual void clear() noexcept = 0;
    virtual size_t size() const noexcept = 0;
    virtual bool empty() const noexcept = 0;
    virtual void reserve(size_t capacity) = 0;
    virtual void shrink() = 0;
    virtual size_t capacity() const noexcept = 0;

    virtual ~Complex() = default;
  };
}
#endif