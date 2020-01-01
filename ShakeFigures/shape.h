#pragma once
#include "stdafx.h"

class CShape {
 public:
  CShape(CPoint centerPoint,
         short width,
         short height,
         short speed,
         CRect allowedMoveArea)
      : center_point_(centerPoint),
        width_(width),
        height_(height),
        speed_(speed),
        allowed_move_area_rect_(allowedMoveArea) {
    color_ = GetRandomColor();
    move_x_ = move_y_ = speed_;
    CalculateShapeRect();
  }

  CShape(short centerPointX,
         short centerPointY,
         short width,
         short height,
         short speed,
         CRect allowedMoveArea)
      : width_(width),
        height_(height),
        speed_(speed),
        allowed_move_area_rect_(allowedMoveArea) {
    color_ = GetRandomColor();
    center_point_.x = centerPointX;
    center_point_.x = centerPointY;

    move_x_ = move_y_ = speed_;
    CalculateShapeRect();
  }

  CShape(const CShape& rhs) {
    center_point_ = rhs.center_point_;
    rect_ = rhs.rect_;
    width_ = rhs.width_;
    height_ = rhs.height_;
    speed_ = rhs.speed_;
    allowed_move_area_rect_ = rhs.allowed_move_area_rect_;
    move_x_ = move_y_ = speed_;
    color_ = rhs.color_;
  }

  CShape& operator=(const CShape& rhs) {
    if (&rhs != this) {
      center_point_ = rhs.center_point_;
      rect_ = rhs.rect_;
      width_ = rhs.width_;
      height_ = rhs.height_;
      speed_ = rhs.speed_;
      allowed_move_area_rect_ = rhs.allowed_move_area_rect_;
      move_x_ = move_y_ = speed_;
      color_ = rhs.color_;
    }

    return *this;
  }

  CShape(CShape&& rhs) {
    center_point_ = rhs.center_point_;
    rect_ = rhs.rect_;
    width_ = rhs.width_;
    height_ = rhs.height_;
    speed_ = rhs.speed_;
    allowed_move_area_rect_ = rhs.allowed_move_area_rect_;
    move_x_ = move_y_ = speed_;
    color_ = rhs.color_;

    rhs.center_point_ = 0;
    rhs.rect_ = 0;
    rhs.width_ = 0;
    rhs.height_ = 0;
    rhs.speed_ = 0;
    rhs.allowed_move_area_rect_ = 0;
    rhs.color_ = 0;
  }

  CShape& operator=(CShape&& rhs) {
    if (&rhs != this) {
      center_point_ = rhs.center_point_;
      rect_ = rhs.rect_;
      width_ = rhs.width_;
      height_ = rhs.height_;
      speed_ = rhs.speed_;
      allowed_move_area_rect_ = rhs.allowed_move_area_rect_;
      move_x_ = move_y_ = speed_;
      color_ = rhs.color_;

      rhs.center_point_ = 0;
      rhs.rect_ = 0;
      rhs.width_ = 0;
      rhs.height_ = 0;
      rhs.speed_ = 0;
      rhs.allowed_move_area_rect_ = 0;
      rhs.color_ = 0;
    }

    return *this;
  }

  virtual ~CShape() {}

 public:
  void SetNewPos(int x, int y) {
    center_point_.x = x;
    center_point_.y = y;

    CalculateShapeRect();
  }

  void SetNewPos(CPoint point) {
    center_point_ = point;

    CalculateShapeRect();
  }

  void SetWidth(short width) {
    width_ = width;
    CalculateShapeRect();
  }
  void SetHeight(short height) {
    height_ = height;
    CalculateShapeRect();
  }
  void SetSpeed(short speed) { speed_ = speed; }

  int GetWidth() const { return width_; }
  int GetHeight() const { return height_; }
  int GetSpeed() const { return speed_; }

  void SetColor(COLORREF color) { color_ = color; }
  COLORREF GetColor() const { return color_; }

  void SetMoveArea(CRect rc) { allowed_move_area_rect_ = rc; }
  void SetDCpointer(CDC* pdc) { pdc_ = pdc; }

  CRect& GetRect() { return rect_; }

  virtual void Draw() = 0;

  void RedrawShapes(HWND hWnd) { InvalidateRect(hWnd, rect_, TRUE); };

  void Move() {
    center_point_.x += move_x_;
    center_point_.y += move_y_;

    if (center_point_.x > allowed_move_area_rect_.right)
      move_x_ = -move_x_;

    if (center_point_.y > allowed_move_area_rect_.bottom)
      move_y_ = -move_y_;

    if (center_point_.y < allowed_move_area_rect_.top)
      move_y_ = -move_y_;

    if (center_point_.x < allowed_move_area_rect_.left)
      move_x_ = -move_x_;

    CalculateShapeRect();

    rect_.MoveToXY(center_point_);
  }

 protected:
  CPoint center_point_;
  CRect rect_;
  int width_;
  int height_;
  int speed_;
  CDC* pdc_;
  CRect allowed_move_area_rect_;

  int move_x_;
  int move_y_;
  COLORREF color_;

 private:
  std::vector<COLORREF> _colorsArray = {RGB(255, 0, 0), RGB(0, 255, 0),
                                        RGB(0, 0, 255)};
  COLORREF GetRandomColor()  // returns a random specific only color
  {
    srand((unsigned int)time(0));
    auto color_index = rand() % _colorsArray.size();
    auto color = _colorsArray.at(color_index);
    return color;
  }

  void CalculateShapeRect() {
    rect_.left = center_point_.x - width_ / 2;
    rect_.top = center_point_.y - height_ / 2;
    rect_.right = center_point_.x + width_ / 2;
    rect_.bottom = center_point_.y + height_ / 2;
  }
};
