#pragma once
#include "shape.h"

class CRectangleShape : public CShape {
 public:
  CRectangleShape(CPoint centerPoint,
                  short width,
                  short height,
                  short speed,
                  CRect allowedMoveArea)
      : CShape(centerPoint, width, height, speed, allowedMoveArea) {}

  CRectangleShape(short centerPointX,
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

  ~CRectangleShape() {}

  void Draw() {
    CPen workPen(PS_SOLID, 1, color_);
    CBrush workBrush(color_);

    CPen* oldPen = pdc_->SelectObject(&workPen);
    auto oldBrush = pdc_->SelectObject(workBrush);

    pdc_->FillRect(&rect_, &workBrush);

    pdc_->SelectObject(oldBrush);
    pdc_->SelectObject(oldPen);
  }
};