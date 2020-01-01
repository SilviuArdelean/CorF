#pragma once
#include "shape.h"

class CEllipseShape : public CShape {
 public:
  CEllipseShape(CPoint centerPoint,
                short width,
                short height,
                short speed,
                CRect allowedMoveArea)
      : CShape(centerPoint, width, height, speed, allowedMoveArea) {}

  CEllipseShape(short centerPointX,
                short centerPointY,
                short width,
                short height,
                short speed,
                CRect allowedMoveArea)
      : CShape(centerPointX,
               centerPointY,
               width,
               height,
               speed,
               allowedMoveArea) {}

  ~CEllipseShape() {}

  void Draw() {
    CPen workPen(PS_SOLID, 1, color_);
    CBrush workBrush(color_);

    CPen* oldPen = pdc_->SelectObject(&workPen);
    auto oldBrush = pdc_->SelectObject(workBrush);

    pdc_->Ellipse(rect_);

    pdc_->SelectObject(oldBrush);
    pdc_->SelectObject(oldPen);
  }
};
