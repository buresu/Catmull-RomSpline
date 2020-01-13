#pragma once

#include <Eigen/Core>
#include <Eigen/StdVector>

class CatmullRomSpline {
public:
  typedef Eigen::Vector3d Point;
  typedef std::vector<Point, Eigen::aligned_allocator<Point>> PointList;

  CatmullRomSpline();
  explicit CatmullRomSpline(size_t size);
  explicit CatmullRomSpline(const PointList &controls);
  ~CatmullRomSpline();

  bool isValid() const;

  size_t size() const;

  Point control(size_t index) const;
  void setControl(size_t index, const Point &control);

  PointList controls() const;
  void setControls(const PointList &controls);

  void reshape(size_t size);

  Point interpolate(double t) const;

protected:
  static inline Point basis(double t, const Point &p0, const Point &p1,
                            const Point &p2, const Point &p3) {
    return 0.5 * ((2.0 * p1) + (-p0 + p2) * t +
                  (2.0 * p0 - 5.0 * p1 + 4.0 * p2 - p3) * (t * t) +
                  (-p0 + 3.0 * p1 - 3.0 * p2 + p3) * (t * t * t));
  }

  PointList mControls;
};
