#include <iostream>
struct point_t
{
  int x, y;
};

struct rectangle_t
{
  int width, height;
  point_t pos;
};

struct Shape
{
  virtual size_t getArea() const;
  virtual rectangle_t getFrameRect() const;
  virtual void move(int dx, int dy);
  virtual void move(point_t p);
  virtual void scale(int k);
};

int main()
{
  
}
