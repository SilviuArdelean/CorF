#pragma once
#include <vector>
#include "shape_rectangle.h"
#include "shape_ellipse.h"
#include <memory>


#define ANGLE			45.0f

#define SHAPE_HEIGHT	100
#define SHAPE_WIDTH		100


enum SHAPE_TYPE {
	ellipse = 1,
	rectangle
};

class CShapesManager
{
	
public:
	CShapesManager() = delete;
	CShapesManager(CWnd* pWnd, CPoint crtShapePoint, CRect moveAreaRect,
						short shapes_count, short widthShape, short heightShape);
	~CShapesManager();

	std::unique_ptr<CShape>		ShapesFactory(SHAPE_TYPE shape_type, CPoint shapeCenterPoint);

	void		SetShapesWidth(short width);
	void		SetShapesHeight(short height);
	void		SetShapesSpeed(short speed);
	void		SetShapesColor(COLORREF color);
	short		GetShapesWidth() const;
	short		GetShapesHeight() const;
	short		GetShapesSpeed() const;
	COLORREF	GetShapesColor() const;

	void		RedrawShapes(HWND hWnd);
	CRect*		GetMoveAreaRect()  { return &m_moveAreaRect; }
	void		SetMoveAreaRect(CRect rect) { m_moveAreaRect = rect; }
	void		SetShapesMoveArea(CRect rc);
	
	void		SetShapesCount(short count) { m_nShapesCount = count; }
	size_t		GetShapesCount() const { m_vectShapes.size(); }
	void		SetInitialSpeed(short speed) { m_nInitialSpeed = speed; }

	void		InitalizeShapesPositions(SHAPE_TYPE shape_type);
	void		RearangeShapes();
	void		SetDCpointer(CDC* pdc);
	void		Draw();
	void		MoveShapes();
	bool		isPointInRectangle(CPoint point);

private:
	void		ComputeNewPositions(CRect areaRect);

private:
	CPoint		m_crtShapePoint;
	CRect		m_moveAreaRect;
	short		m_nInitialSpeed;
	
	std::vector< std::unique_ptr<CShape> > m_vectShapes;

	short		m_nShapesCount;
	short		m_shapeWidth;
	short		m_shapeHeight;
};

