#include "CatmullRomSpline.hpp"

CatmullRomSpline::CatmullRomSpline() {}

CatmullRomSpline::CatmullRomSpline(size_t size) { mControls.resize(size); }

CatmullRomSpline::CatmullRomSpline(const PointList &controls)
    : mControls(controls) {}

CatmullRomSpline::~CatmullRomSpline() {}

bool CatmullRomSpline::isValid() const { return mControls.size() >= 4; }

size_t CatmullRomSpline::size() const { return mControls.size(); }

CatmullRomSpline::Point CatmullRomSpline::control(size_t index) const {
  return mControls[index];
}

void CatmullRomSpline::setControl(size_t index, const Point &control) {
  mControls[index] = control;
}

CatmullRomSpline::PointList CatmullRomSpline::controls() const {
  return mControls;
}

void CatmullRomSpline::setControls(const PointList &controls) {
  mControls = controls;
}

void CatmullRomSpline::reshape(size_t size) {

  if (size == mControls.size()) {
    return;
  }

  PointList controls(size);

  const double step = 1.0 / double(size - 1);

  for (size_t i = 0; i < size; ++i) {
    controls[i] = interpolate(step * double(i));
  }

  mControls = controls;
}

CatmullRomSpline::Point CatmullRomSpline::interpolate(double t) const {

  double index, tp = 1.0;

  if (t >= 1.0) {
    index = double(mControls.size() - 2);
  } else {
    tp = std::modf(double(mControls.size() - 1) * t, &index);
  }

  Point p0, p1, p2, p3;
  const size_t i = size_t(index);

  p1 = mControls[i];
  p2 = mControls[i + 1];

  if (i == 0) {
    p3 = mControls[i + 2];
    p0 = p3 + 3.0 * (p1 - p2);
  } else if (i == mControls.size() - 2) {
    p0 = mControls[i - 1];
    p3 = p0 + 3.0 * (p2 - p1);
  } else {
    p0 = mControls[i - 1];
    p3 = mControls[i + 2];
  }

  return basis(tp, p0, p1, p2, p3);
}
