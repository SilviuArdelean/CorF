#pragma once
#include <memory>
#include <vector>
#include "shape_ellipse.h"
#include "shape_rectangle.h"

#define ANGLE 45.0f

#define SHAPE_HEIGHT 100
#define SHAPE_WIDTH 100

enum SHAPE_TYPE { ellipse = 1, rectangle };

class CShapesManager {
 public:
  CShapesManager() = delete;
  CShapesManager(CWnd* pWnd,
                 CPoint crtShapePoint,
                 CRect moveAreaRect,
                 short shapes_count,
                 short widthShape,
                 short heightShape);
  ~CShapesManager();

  std::unique_ptr<CShape> ShapesFactory(SHAPE_TYPE shape_type,
                                        CPoint shapeCenterPoint);

  void SetShapesWidth(short width);
  void SetShapesHeight(short height);
  void SetShapesSpeed(short speed);
  void SetShapesColor(COLORREF color);
  short GetShapesWidth() const;
  short GetShapesHeight() const;
  short GetShapesSpeed() const;
  COLORREF GetShapesColor() const;

  void RedrawShapes(HWND hWnd);
  CRect* GetMoveAreaRect() { return &move_area_rect_; }
  void SetMoveAreaRect(CRect rect) { move_area_rect_ = rect; }
  void SetShapesMoveArea(CRect rc);

  void SetShapesCount(short count) { shapes_count_ = count; }
  size_t GetShapesCount() const { vect_shapes_.size(); }
  void SetInitialSpeed(short speed) { initial_speed_ = speed; }

  void InitalizeShapesPositions(SHAPE_TYPE shape_type);
  void RearangeShapes();
  void SetDCpointer(CDC* pdc);
  void Draw();
  void MoveShapes();
  bool isPointInRectangle(CPoint point);

 private:
  void ComputeNewPositions(CRect areaRect);

 private:
  CPoint crt_shape_point_;
  CRect move_area_rect_;
  short initial_speed_;

  std::vector<std::unique_ptr<CShape>> vect_shapes_;

  short shapes_count_;
  short shape_width_;
  short shape_height_;
};
