#ifndef CPLXSHAPE_HPP
#define CPLXSHAPE_HPP
#include "complex.hpp"

namespace shaykhraziev
{
  struct CplxShape: Shape, Complex
  {
    size_t shapes_size = 0, shapes_cap = 0;
    Shape ** shapes = nullptr;
    point_t center = {0.0, 0.0};

    double getArea() const override;
    rectangle_t getFrameRect() const override;
    void move(point_t to) override;
    void move(double dx, double dy) override;
    void scale(double coef) override;

    void preappend(Shape* shp) override;
    void append(Shape* shp) override;
    void add(Shape* shp, size_t index) override;

    Shape* last() override;
    Shape* first() override;
    Shape* get(size_t index) noexcept override;
    Shape* at(size_t index) override;

    const Shape* last() const override;
    const Shape* first() const override;
    const Shape* get(size_t index) const noexcept override;
    const Shape* at(size_t index) const override;

    void remove(size_t index) override;
    void dropFirst() noexcept override;
    void dropLast() noexcept override;
    void clear() noexcept override;

    size_t size() const noexcept override;
    bool empty() const noexcept override;
    void reserve(size_t capacity) override;
    void shrink() override;
    size_t capacity() const noexcept override;

    ~CplxShape() override;
    explicit CplxShape();
  };

  Shape** extend(Shape** curr, size_t cap, size_t add);
  Shape** addAt(Shape** curr, Shape* shp, size_t& size, size_t& capacity, size_t index);
  Shape** rmAt(Shape** curr, size_t& size, size_t index);
  point_t calcCenter(Shape** curr, size_t size);

}
#endif