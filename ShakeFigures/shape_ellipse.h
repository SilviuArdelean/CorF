#pragma once
#include "shape.h"

class CEllipseShape : public CShape
{
  public:
	CEllipseShape(CPoint centerPoint, short width, short height, short speed, CRect allowedMoveArea)
		: CShape(centerPoint, width, height, speed, allowedMoveArea)
	{
	}

	CEllipseShape(short centerPointX, short centerPointY, short width, short height, short speed, CRect allowedMoveArea)
		: CShape(centerPointX, centerPointY, width, height, speed, allowedMoveArea)
	{
	}

	~CEllipseShape() {}

	void Draw()
	{
		CPen workPen(PS_SOLID, 1, _color);
		CBrush workBrush(_color);

		CPen* oldPen = _pDC->SelectObject(&workPen);
		auto oldBrush = _pDC->SelectObject(workBrush);

		_pDC->Ellipse(_rect);

		_pDC->SelectObject(oldBrush);
		_pDC->SelectObject(oldPen);
	}
};
