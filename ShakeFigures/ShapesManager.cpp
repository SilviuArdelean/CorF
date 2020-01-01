#include "stdafx.h"

#include "ShapesManager.h"
#include <cassert>

CShapesManager::CShapesManager(CWnd* pWnd,
                               CPoint crtShapePoint,
                               CRect moveAreaRect,
                               short shapes_count,
                               short widthShape,
                               short heightShape)
    : shape_width_(widthShape),
      shape_height_(heightShape),
      shapes_count_(shapes_count),
      initial_speed_(40) {
  assert(shapes_count > 0 && shapes_count <= 4);

  crt_shape_point_ = crtShapePoint;

  move_area_rect_ = moveAreaRect;
}

CShapesManager::~CShapesManager() {}

std::unique_ptr<CShape> CShapesManager::ShapesFactory(SHAPE_TYPE shape_type,
                                                      CPoint shapeCenterPoint) {
  switch (shape_type) {
    case SHAPE_TYPE::ellipse:
      return std::make_unique<CEllipseShape>(shapeCenterPoint, shape_width_,
                                             shape_height_, initial_speed_,
                                             move_area_rect_);
      break;

    case SHAPE_TYPE::rectangle:
    default:
      return std::make_unique<CRectangleShape>(shapeCenterPoint, shape_width_,
                                               shape_height_, initial_speed_,
                                               move_area_rect_);
      break;
  }
}

void CShapesManager::InitalizeShapesPositions(SHAPE_TYPE shape_type) {
  if (vect_shapes_.empty())
    vect_shapes_.clear();

  switch (shapes_count_) {
    case 1: {
      CPoint p1(static_cast<int>(move_area_rect_.right / 2),
                static_cast<int>(move_area_rect_.bottom / 2));
      vect_shapes_.emplace_back(ShapesFactory(shape_type, p1));
    } break;
    case 2: {
      CPoint p1(static_cast<int>(move_area_rect_.right / 4),
                static_cast<int>(move_area_rect_.bottom / 2));
      CPoint p2(
          static_cast<int>(move_area_rect_.right / 2 + move_area_rect_.right / 4),
          static_cast<int>(move_area_rect_.bottom / 2));

      vect_shapes_.emplace_back(ShapesFactory(shape_type, p1));
      vect_shapes_.emplace_back(ShapesFactory(shape_type, p2));
    } break;
    case 3: {
      CPoint p1(static_cast<int>(move_area_rect_.right / 4),
                static_cast<int>(move_area_rect_.bottom / 4));
      CPoint p2(
          static_cast<int>(move_area_rect_.right / 2 + move_area_rect_.right / 4),
          static_cast<int>(move_area_rect_.bottom / 4));
      CPoint p3(static_cast<int>(move_area_rect_.right / 2),
                static_cast<int>(move_area_rect_.bottom / 2 +
                                 move_area_rect_.bottom / 4));

      vect_shapes_.emplace_back(ShapesFactory(shape_type, p1));
      vect_shapes_.emplace_back(ShapesFactory(shape_type, p2));
      vect_shapes_.emplace_back(ShapesFactory(shape_type, p3));
    } break;
    case 4:
    default: {
      CPoint p1(static_cast<int>(move_area_rect_.right / 4),
                static_cast<int>(move_area_rect_.bottom / 4));
      CPoint p2(
          static_cast<int>(move_area_rect_.right / 2 + move_area_rect_.right / 4),
          static_cast<int>(move_area_rect_.bottom / 4));
      CPoint p3(static_cast<int>(move_area_rect_.right / 4),
                static_cast<int>(move_area_rect_.bottom / 2 +
                                 move_area_rect_.bottom / 4));
      CPoint p4(
          static_cast<int>(move_area_rect_.right / 2 + move_area_rect_.right / 4),
          static_cast<int>(move_area_rect_.bottom / 2 +
                           move_area_rect_.bottom / 4));

      vect_shapes_.emplace_back(ShapesFactory(shape_type, p1));
      vect_shapes_.emplace_back(ShapesFactory(shape_type, p2));
      vect_shapes_.emplace_back(ShapesFactory(shape_type, p3));
      vect_shapes_.emplace_back(ShapesFactory(shape_type, p4));
    } break;
  }
}

void CShapesManager::SetDCpointer(CDC* pdc) {
  for (auto& shape : vect_shapes_) {
    shape->SetDCpointer(pdc);
  }
}

void CShapesManager::SetShapesWidth(short width) {
  if (vect_shapes_.empty())
    return;

  for (auto it = vect_shapes_.begin(); it != vect_shapes_.end(); ++it) {
    (*it)->SetWidth(width);
  }
}

void CShapesManager::SetShapesHeight(short height) {
  if (vect_shapes_.empty())
    return;

  for (auto it = vect_shapes_.begin(); it != vect_shapes_.end(); ++it) {
    (*it)->SetHeight(height);
  }
}

void CShapesManager::SetShapesSpeed(short speed) {
  if (vect_shapes_.empty())
    return;

  for (auto it = vect_shapes_.begin(); it != vect_shapes_.end(); ++it) {
    (*it)->SetSpeed(speed);
  }
}

void CShapesManager::SetShapesMoveArea(CRect rc) {
  if (vect_shapes_.empty())
    return;

  for (auto it = vect_shapes_.begin(); it != vect_shapes_.end(); ++it) {
    (*it)->SetMoveArea(rc);
  }
}

void CShapesManager::RedrawShapes(HWND hWnd) {
  for (auto it = vect_shapes_.begin(); it != vect_shapes_.end(); ++it) {
    (*it)->RedrawShapes(hWnd);
  }
}

void CShapesManager::SetShapesColor(COLORREF color) {
  if (vect_shapes_.empty())
    return;

  for (auto it = vect_shapes_.begin(); it != vect_shapes_.end(); ++it) {
    (*it)->SetColor(color);
  }
}

short CShapesManager::GetShapesWidth() const {
  if (vect_shapes_.empty())
    return 0;

  return vect_shapes_.at(0)->GetWidth();
}

short CShapesManager::GetShapesHeight() const {
  if (vect_shapes_.empty())
    return 0;

  return vect_shapes_.at(0)->GetHeight();
}

short CShapesManager::GetShapesSpeed() const {
  if (vect_shapes_.empty())
    return 0;

  return vect_shapes_.at(0)->GetSpeed();
}

COLORREF CShapesManager::GetShapesColor() const {
  if (vect_shapes_.empty())
    return 0;

  return vect_shapes_.at(0)->GetColor();
}

void CShapesManager::ComputeNewPositions(CRect areaRect) {
  if (vect_shapes_.empty())
    return;

  auto shapes_count = vect_shapes_.size();
  assert(shapes_count > 0 && shapes_count <= 4);

  switch (shapes_count) {
    case 1: {
      vect_shapes_.at(0)->SetNewPos(static_cast<int>(areaRect.right / 2),
                                    static_cast<int>(areaRect.bottom / 2));
    } break;
    case 2: {
      vect_shapes_.at(0)->SetNewPos(static_cast<int>(areaRect.right / 4),
                                    static_cast<int>(areaRect.bottom / 2));
      vect_shapes_.at(1)->SetNewPos(
          static_cast<int>(areaRect.right / 2 + areaRect.right / 4),
          static_cast<int>(areaRect.bottom / 2));
    } break;
    case 3: {
      vect_shapes_.at(0)->SetNewPos(static_cast<int>(areaRect.right / 4),
                                    static_cast<int>(areaRect.bottom / 4));
      vect_shapes_.at(1)->SetNewPos(
          static_cast<int>(areaRect.right / 2 + areaRect.right / 4),
          static_cast<int>(areaRect.bottom / 4));
      vect_shapes_.at(2)->SetNewPos(
          static_cast<int>(areaRect.right / 2),
          static_cast<int>(areaRect.bottom / 2 + areaRect.bottom / 4));
    } break;
    case 4:
    default: {
      vect_shapes_.at(0)->SetNewPos(static_cast<int>(areaRect.right / 4),
                                    static_cast<int>(areaRect.bottom / 4));
      vect_shapes_.at(1)->SetNewPos(
          static_cast<int>(areaRect.right / 2 + areaRect.right / 4),
          static_cast<int>(areaRect.bottom / 4));
      vect_shapes_.at(2)->SetNewPos(
          static_cast<int>(areaRect.right / 4),
          static_cast<int>(areaRect.bottom / 2 + areaRect.bottom / 4));
      vect_shapes_.at(3)->SetNewPos(
          static_cast<int>(areaRect.right / 2 + areaRect.right / 4),
          static_cast<int>(areaRect.bottom / 2 + areaRect.bottom / 4));
    } break;
  }
}

void CShapesManager::Draw() {
  for (auto& shape : vect_shapes_) {
    shape->Draw();
  }
}

void CShapesManager::MoveShapes() {
  for (auto& shape : vect_shapes_) {
    shape->Move();
  }
}

bool CShapesManager::isPointInRectangle(CPoint point) {
  for (auto& shape : vect_shapes_) {
    if (shape->GetRect().PtInRect(point))
      return true;
  }
  return false;
}

void CShapesManager::RearangeShapes() {
  ComputeNewPositions(move_area_rect_);
}
