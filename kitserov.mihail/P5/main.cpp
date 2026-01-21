#include <iostream>

int main()
{
  using namespace kitserov;
  const size_t count = 3;
  Shape* shapes[count] = {};
  const char* shapeNames[] = {"Rectangle", "Xquare", "Polygon"};

  try {
    shapes[0] = new Rectangle{{0.0, 0.0}, 1.0, 2.0};
    shapes[1] = new Xquare{{1.0, 0.0}, 2.0};
    point_t triangleVertices[3] = {{3.0, 0.0}, {4.0, 2.0}, {5.0, 0.0}};
    shapes[2] = new Polygon{triangleVertices, 3};
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << "\n";
    for (size_t i = 0; i < count; i++) {
      delete shapes[i];
    }
    return 2;
  }

  std::cout << "BEFORE SCALING\n";
  printAllInfo(std::cout, shapes, shapeNames, count);

  std::cout << "Point for scale:\n";
  float xx = 0.0;
  float yy = 0.0;
  std::cout << "x = ";
  std::cin >> xx;
  std::cout << "y = ";
  std::cin >> yy;
  float k = 0;
  std::cout << "Ratio: ";
  std::cin >> k;
  if (!std::cin) {
    std::cerr << "incorrect input\n";
    for (size_t i = 0; i < count; i++) {
      delete shapes[i];
    }
    return 1;
  }
  for (size_t i = 0; i < count; i++) {
    try {
      scalePoint(shapes[i], {xx, yy}, k);
    } catch (const std::exception& e) {
      std::cerr << "Error: " << e.what() << "\n";
      for (size_t i = 0; i < count; i++) {
        delete shapes[i];
      }
      return 2;
    }
  }
  std::cout << "AFTER SCALING\n";
  printAllInfo(std::cout, shapes, shapeNames, count);

  for (size_t i = 0; i < count; i++) {
    delete shapes[i];
  }
  return 0;
}


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

void kitserov::Polygon::scale(float k) noexcept
{
  for (size_t i = 0; i < vertexCount_; ++i) {
    vertices_[i].x = center_.x + (vertices_[i].x - center_.x) * k;
    vertices_[i].y = center_.y + (vertices_[i].y - center_.y) * k;
  }
}

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
  if (k <= 0) {
    throw std::logic_error("bad ratio");
  }
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
