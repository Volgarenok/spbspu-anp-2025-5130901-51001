#include "cplxshape.hpp"
#include "../common/rectangle.hpp"
#include "../common/concave.hpp"
#include "../common/polygon.hpp"
#include "../common/utils.hpp"
#include <iostream>

int main()
{
  using namespace shaykhraziev;

  int err = 0;
  size_t shp_cnt = 2;
  Shape* shps[2] = {};

  try {
    shps[0] = new Rectangle({0, 0}, {10, 10});

    CplxShape* cplx = new CplxShape();

    shps[1] = cplx;

    cplx->append(new Rectangle({3, 3, {-4, -6}}));

    point_t pts[] = {{0, 1}, {4, 6}, {-1, -10}, {-5, -5}};
    cplx->append(new Polygon(pts, 4));

    point_t pts2[] = {{0, 5}, {5, 1}, {1, 0}, {2, 2}};
    cplx->append(new Concave(pts2, 4));


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