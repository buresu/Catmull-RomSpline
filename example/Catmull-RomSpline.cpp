#include <CatmullRomSpline.hpp>
#include <QApplication>
#include <QMouseEvent>
#include <QPainter>
#include <QRandomGenerator>
#include <QWidget>

class SplineWidget : public QWidget {
public:
  explicit SplineWidget(QWidget *parent = nullptr);
  ~SplineWidget() {}

protected:
  virtual void paintEvent(QPaintEvent *e) override;
  virtual void mousePressEvent(QMouseEvent *e) override;
  virtual void mouseMoveEvent(QMouseEvent *e) override;
  virtual void mouseReleaseEvent(QMouseEvent *e) override;

private:
  QList<QPointF> mControls;
  int mSelectIndex = -1;
};

SplineWidget::SplineWidget(QWidget *parent) : QWidget(parent) {

  for (int i = 0; i < 6; ++i) {
    auto x = QRandomGenerator::global()->generateDouble() * width();
    auto y = QRandomGenerator::global()->generateDouble() * height();
    x = qMin(qMax(10.0, x), double(width() - 10));
    y = qMin(qMax(10.0, y), double(width() - 10));
    mControls.append(QPointF(x, y));
  }

  std::sort(mControls.begin(), mControls.end(),
            [](auto const &lhs, auto const &rhs) { return lhs.x() > rhs.x(); });
}

void SplineWidget::paintEvent(QPaintEvent *) {

  QPainter p(this);

  p.setRenderHint(QPainter::Antialiasing);

  p.fillRect(rect(), Qt::white);

  // draw catmull-rom spline
  CatmullRomSpline::PointList controls;

  for (auto &c : mControls) {
    controls.push_back(CatmullRomSpline::Point(c.x(), c.y(), 0));
  }

  CatmullRomSpline cs(controls);
  QPolygonF path;

  for (int i = 0; i <= 100; ++i) {
    auto pt = cs.interpolate(double(i) / 100.0);
    path.append(QPointF(pt.x(), pt.y()));
  }

  p.setPen(Qt::red);
  p.drawPolyline(path);

  // draw controls
  for (int i = 0; i < mControls.size(); ++i) {
    mSelectIndex == i ? p.setPen(Qt::magenta) : p.setPen(Qt::black);
    p.drawEllipse(mControls[i], 3, 3);
  }
}

void SplineWidget::mousePressEvent(QMouseEvent *e) {

  if (e->button() == Qt::LeftButton) {
    for (int i = 0; i < mControls.size(); ++i) {
      auto d = e->localPos() - mControls[i];
      if (d.x() * d.x() + d.y() * d.y() < 6 * 6) {
        mSelectIndex = i;
        break;
      }
    }
  }

  e->ignore();
  update();
}

void SplineWidget::mouseMoveEvent(QMouseEvent *e) {

  if (mSelectIndex >= 0) {
    mControls[mSelectIndex] = e->localPos();
  }

  e->ignore();
  update();
}

void SplineWidget::mouseReleaseEvent(QMouseEvent *e) {
  mSelectIndex = -1;
  e->ignore();
  update();
}

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  SplineWidget w;
  w.show();
  return a.exec();
}
