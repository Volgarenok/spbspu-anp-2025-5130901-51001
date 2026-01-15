#include <iostream>
#include "../common/rectangle.hpp"
#include "../common/polygon.hpp"
#include "../common/concave.hpp"
#include "../common/utils.hpp"

int main()
{
  using namespace shaykhraziev;

  int err = 0;
  size_t shp_cnt = 4;
  Shape* shps[4] = {};

  try {
    shps[0] = new Rectangle({0, 0}, {10, 10});
    shps[1] = new Rectangle({3, 3, {-4, -6}});

    point_t pts[] = {{0, 1}, {4, 6}, {-1, -10}, {-5, -5}};
    shps[2] = new Polygon(pts, 4);

    point_t pts2[] = {{0, 5}, {5, 1}, {1, 0}, {2, 2}};
    shps[3] = new Concave(pts2, 4);
  } catch (...) {
    std::cerr << "memalloc error" << "\n";
    err = 1;
  }

  if (err == 0) {
    outputParams(std::cout, shps, shp_cnt);

    double x = 0;
    double y = 0;
    double scale = 0;

    if (std::cin >> x >> y >> scale && scale >= 0) {
      for (size_t i = 0; i < shp_cnt; i++) {
        scaleRelative(*shps[i], {x, y}, scale);
      }
      outputParams(std::cout, shps, shp_cnt);
    } else {
      std::cerr << "bad input" << "\n";
      err = 1;
    }
  }

  removeArray(shps, shp_cnt);

  return err;
}
