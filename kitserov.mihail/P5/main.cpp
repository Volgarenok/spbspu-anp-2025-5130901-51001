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
  struct Xquare : Shape
  {
    Xquare(point_t p, float s);
    float getArea() const override;
    rectangle_t getFrameRect() const override;
    void move(float dx, float dy) override;
    void move(point_t p) override;
    void scale(float k) override;
  private:
    point_t centre;
    float side;
  };
  struct Polygon : Shape
  {
    Polygon(point_t* vertices, size_t vertexCount);
    float getArea() const override;
    rectangle_t getFrameRect() const override;
    void move(float dx, float dy) override;
    void move(point_t p) override;
    void scale(float k) override;
    ~Polygon();
  private:
    point_t* vertices_;
    size_t vertexCount_;
    point_t center_;
  };
  rectangle_t frame(const point_t* pts, size_t s);
  void frameOutput(std::ostream& os, rectangle_t fr);
  void shapeOutput(std::ostream& os, Shape* sh, const char* name);
  void scalePoint(Shape* sh, const point_t& p, float k);
  rectangle_t getOverallFrame(Shape** shapes, size_t count);
}

int main()
{
  using namespace kitserov;
  size_t count = 3;
  Shape* shapes[count] = {};
  try {
    shapes[0] = new Rectangle({0.0, 0.0}, 1.0, 2.0);
    shapes[1] = new Xquare({1.0, 0.0}, 2.0);
    point_t triangleVertices[3] = {{3.0, 0.0}, {4.0, 2.0}, {5.0, 0.0}};
    shapes[2] = new Polygon(triangleVertices, 3);
  } catch (...) {
    std::cerr << "Error!\n";
    for (size_t i = 0; i < count; i++) {
      delete shapes[i];
    }
    return 2;
  }
  float totalAreaPrevious = 0.0;
  for (size_t i = 0; i < count; i++) {
    totalAreaPrevious += shapes[i]->getArea();
  }
  shapeOutput(std::cout, shapes[0], "Rectangle");
  shapeOutput(std::cout, shapes[1], "Xquare");
  shapeOutput(std::cout, shapes[2], "Polygon");
  std::cout << "Total area = " << totalAreaPrevious << "\n";
  rectangle_t overallFrame = getOverallFrame(shapes, count);
  std::cout << "Overall frame:\n";
  frameOutput(std::cout, overallFrame);
  float k = 0;
  std::cin >> k;
  if (!std::cin || k <= 0) {
    std::cerr << "incorrect input\n";
    for (size_t i = 0; i < count; i++) {
      delete shapes[i];
    }
    return 1;
  }
  float totalAreaAfter = 0.0;
  for (size_t i = 0; i < count; i++) {
    scalePoint(shapes[i], {1.0, 0.0}, k);
    totalAreaAfter += shapes[i]->getArea();
  }
  shapeOutput(std::cout, shapes[0], "Rectangle");
  shapeOutput(std::cout, shapes[1], "Xquare");
  shapeOutput(std::cout, shapes[2], "Polygon");
  std::cout << "Total area = " << totalAreaAfter << "\n";
  overallFrame = getOverallFrame(shapes, count);
  std::cout << "Overall frame:\n";
  frameOutput(std::cout, overallFrame);
  for (size_t i = 0; i < count; i++) {
    delete shapes[i];
  }
  return 0;
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
  rect.width *= k;
  rect.height *= k;
}

kitserov::rectangle_t kitserov::frame(const point_t* pts, size_t s)
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
  float w = maxx - minx;
  float h = maxy - miny;
  point_t p = {minx + w/2, miny + h/2};
  return rectangle_t{w, h, p};
}

void kitserov::frameOutput(std::ostream& os, rectangle_t fr)
{
  float left = fr.pos.x - fr.width / 2;
  float top = fr.pos.y + fr.height / 2;
  float right = fr.pos.x + fr.width / 2;
  float bottom = fr.pos.y - fr.height / 2;
  os << "Frame (leftTop " << left << ", " << top << " rightBot " << right << ", " << bottom << ")\n";
}

void kitserov::shapeOutput(std::ostream& os, Shape* sh, const char* name)
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

kitserov::Xquare::Xquare(point_t p, float s) :
  centre(p),
  side(s)
{}

float kitserov::Xquare::getArea() const
{
  return 0.5 * side * side;
}

kitserov::rectangle_t kitserov::Xquare::getFrameRect() const
{
  return {side, side, centre};
}

void kitserov::Xquare::move(float dx, float dy)
{
  centre.x += dx;
  centre.y += dy;
}

void kitserov::Xquare::move(point_t p)
{
  centre = p;
}

void kitserov::Xquare::scale(float k)
{
  side *= k;
}

kitserov::Polygon::Polygon(point_t* vertices, size_t vertexCount) :
  vertices_(new point_t[vertexCount]),
  vertexCount_(vertexCount)
{
  for (size_t i = 0; i < vertexCount; ++i) {
    vertices_[i] = vertices[i];
  }
  float sumX = 0, sumY = 0;
  for (size_t i = 0; i < vertexCount_; ++i) {
    sumX += vertices_[i].x;
    sumY += vertices_[i].y;
  }
  center_ = {sumX / vertexCount_, sumY / vertexCount_};
}

kitserov::Polygon::~Polygon()
{
  delete[] vertices_;
}

float kitserov::Polygon::getArea() const
{
  float area = 0;
  for (size_t i = 0; i < vertexCount_; ++i) {
    size_t j = (i + 1) % vertexCount_;
    area += vertices_[i].x * vertices_[j].y - vertices_[j].x * vertices_[i].y;
  }
  return std::abs(area) / 2.0;
}

kitserov::rectangle_t kitserov::Polygon::getFrameRect() const
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
  point_t center = {minX + width/2, minY + height/2};
  return {width, height, center};
}

void kitserov::Polygon::move(float dx, float dy)
{
  for (size_t i = 0; i < vertexCount_; ++i) {
    vertices_[i].x += dx;
    vertices_[i].y += dy;
  }
  center_.x += dx;
  center_.y += dy;
}

void kitserov::Polygon::move(point_t p)
{
  float dx = p.x - center_.x;
  float dy = p.y - center_.y;
  move(dx, dy);
}

void kitserov::Polygon::scale(float k)
{
  for (size_t i = 0; i < vertexCount_; ++i) {
    vertices_[i].x = center_.x + (vertices_[i].x - center_.x) * k;
    vertices_[i].y = center_.y + (vertices_[i].y - center_.y) * k;
  }
}

kitserov::rectangle_t kitserov::getOverallFrame(Shape** shapes, size_t count)
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
  point_t center = {minX + width / 2, minY + height / 2};
  return {width, height, center};
}
