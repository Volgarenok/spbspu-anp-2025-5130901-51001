#include <iostream>
namespace kitserov
{
  struct point_t
  {
    float x, y;
  };

  struct rectangle_t
  {
    float width, height;
    point_t pos;
  };

  struct Shape
  {
    virtual float getArea() const = 0;
    virtual rectangle_t getFrameRect() const = 0;
    virtual void move(float dx, float dy) = 0;
    virtual void move(point_t p) = 0;
    virtual void scale(float k) = 0;
    virtual ~Shape() = default;
  };

  struct Rectangle : Shape
  {
    Rectangle(point_t p, float w, float h);
    float getArea() const override;
    rectangle_t getFrameRect() const override;
    void move(float dx, float dy) override;
    void move(point_t p) override;
    void scale(float k) override;
  private:
    rectangle_t rect;
  };
  point_t * extend(const point_t * pts, size_t s, point_t fill);
  void extend(point_t ** pts, size_t & s, point_t fill);
  void append(const Shape * sh, point_t ** ppts, size_t & s);
  rectangle_t frame(const point_t * pts, size_t s);
  char * canvas(rectangle_t fr, char fill);
  void paint(point_t p, char* cnv, rectangle_t fr, char fill);
  void flush(std::ostream& os, const char * cnv, rectangle_t fr);
}

int main()
{

}

kitserov::Rectangle::Rectangle(point_t p, float w, float h):
  rect{w, h, p}
{}

float kitserov::Rectangle::getArea() const
{
  return rect.width * rect.height;
}
kitserov::rectangle_t kitserov::Rectangle::getFrameRect() const
{
  return rect;
}
void kitserov::Rectangle::move(float dx, float dy)
{
  rect.pos.x += dx;
  rect.pos.y += dy;
}
void kitserov::Rectangle::move(point_t p)
{
  rect.pos = p;
}
void kitserov::Rectangle::scale(float k)
{
  if (k <= 0){
    throw std::logic_error("bad ratio");
  }
  rect.width *= k;
  rect.height *= k;
}
void kitserov::paint(point_t p, char * cnv, rectangle_t fr, char fill)
{
  int dx = static_cast<int>(p.x - (fr.pos.x - fr.width / 2));
  int dy = static_cast<int>(fr.pos.y + fr.height / 2 - p.y);
  if (dx >= 0 && dx < static_cast<int>(fr.width) && dy >= 0 && dy < static_cast<int>(fr.height))
  {
    cnv[dy * static_cast<int>(fr.width) + dx] = fill;
  }
}
kitserov::point_t * kitserov::extend(const point_t * pts, size_t s, point_t fill)
{
  point_t * r = new point_t[s + 1];
  for (size_t i = 0; i < s; ++i){
    r[i] = pts[i];
  }
  r[s] = fill;
  return r;
}
void kitserov::extend(point_t ** pts, size_t & s, point_t fill)
{
  point_t * r = extend(*pts, s, fill);
  delete[] *pts;
  ++s;
  *pts = r;
}
void kitserov::flush(std::ostream& os, const char * cnv, rectangle_t fr)
{
  for (size_t i = 0; i < fr.height; ++i){
    for (size_t j = 0; j < fr.width; ++j){
      os << cnv[i * static_cast<int>(fr.width) + j];
    }
    os << '\n';
  }
}
char * kitserov::canvas(rectangle_t fr, char fill)
{
  size_t s = fr.width * fr.height;
  char * c = new char [s];
  for (size_t i = 0; i < s; ++i){
    c[i] = fill;
  }
  return c;
}
kitserov::rectangle_t kitserov::frame(const point_t * pts, size_t s)
{
  float minx = pts[0].x;
  float miny = pts[0].y;
  float maxx = pts[0].x;
  float maxy = pts[0].y;
  for (size_t i = 0; i < s; ++i){
    minx = std::min(minx, pts[i].x);
    miny = std::min(miny, pts[i].y);
    maxx = std::max(maxx, pts[i].x);
    maxy = std::max(maxy, pts[i].y);
  }
  float w = maxx - minx;
  float h = maxy - miny;
  point_t p = {minx + w/2, miny + h/2};
  return rectangle_t{w, h, p};
}
