#include <iostream>
namespace kitserov
{
  struct point_t
  {
    int x, y;
  };

  struct rectangle_t
  {
    size_t width, height;
    point_t pos;
  };

  struct Shape
  {
    virtual size_t getArea() const;
    virtual rectangle_t getFrameRect() const;
    virtual void move(int dx, int dy);
    virtual void move(point_t p);
    virtual void scale(int k);
    virtual ~Shape();
  };

  struct Rectangle : Shape
  {
    Rectangle(point_t p, size_t w, size_t h);
    size_t getArea() const override;
    rectangle_t getFrameRect() const override;
    void move(int dx, int dy) override;
    void move(point_t p) override;
    void scale(int k) override;
  private:
    rectangle_t rect;
  };
}

int main()
{

}

kitserov::Rectangle::Rectangle(point_t p, size_t w, size_t h):
  rect{w, h, p}
{}

size_t kitserov::Rectangle::getArea() const
{
  return rect.width * rect.height;
}
kitserov::rectangle_t kitserov::Rectangle::getFrameRect() const
{
  return rect;
}
void kitserov::Rectangle::move(int dx, int dy)
{
  rect.pos.x += dx;
  rect.pos.y += dy;
}
void kitserov::Rectangle::move(point_t p)
{
  rect.pos.x += p.x;
  rect.pos.y += p.y;
}
void kitserov::Rectangle::scale(int k)
{
  if (k <= 0){
    throw std::logic_error("bad ratio");
  }
  rect.width *= k;
  rect.height *= k;
}
