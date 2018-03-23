#include "stdafx.h"
#include "ShapesManager.h"
#include <cassert>


CShapesManager::CShapesManager(CWnd* pWnd, CPoint crtShapePoint, CRect moveAreaRect,
									short shapes_count, short widthShape, short heightShape)
	: m_shapeWidth(widthShape)
	, m_shapeHeight(heightShape)
	, m_nShapesCount(shapes_count)
	, m_nInitialSpeed(40)
{
	assert(shapes_count > 0 && shapes_count <= 4);

	m_crtShapePoint = crtShapePoint;
	
	m_moveAreaRect = moveAreaRect;
}


CShapesManager::~CShapesManager()
{
}


std::unique_ptr<CShape> CShapesManager::ShapesFactory(SHAPE_TYPE shape_type, CPoint shapeCenterPoint)
{
	switch (shape_type)
	{
		case SHAPE_TYPE::ellipse:
			return std::make_unique<CEllipseShape>(shapeCenterPoint,
													m_shapeWidth, m_shapeHeight, 
													m_nInitialSpeed, m_moveAreaRect);
			break;

		case SHAPE_TYPE::rectangle:
		default:
			return std::make_unique<CRectangleShape>(shapeCenterPoint, 
													m_shapeWidth, m_shapeHeight, 
													m_nInitialSpeed, m_moveAreaRect);
			break;
	}
}

void CShapesManager::InitalizeShapesPositions(SHAPE_TYPE shape_type)
{
	if (m_vectShapes.empty()) 
		m_vectShapes.clear();

	switch (m_nShapesCount)
	{
		case 1:
			{
				CPoint p1(static_cast<int>(m_moveAreaRect.right / 2), static_cast<int>(m_moveAreaRect.bottom / 2));
				m_vectShapes.emplace_back( ShapesFactory(shape_type, p1) );
			}
			break;
		case 2:
			{
				CPoint p1(static_cast<int>(m_moveAreaRect.right / 4), 
												static_cast<int>(m_moveAreaRect.bottom / 2));
				CPoint p2(static_cast<int>(m_moveAreaRect.right / 2 + m_moveAreaRect.right / 4), 
												static_cast<int>(m_moveAreaRect.bottom / 2));

				m_vectShapes.emplace_back( ShapesFactory(shape_type, p1) );
				m_vectShapes.emplace_back( ShapesFactory(shape_type, p2) );
			}
			break;
		case 3:
			{
				CPoint p1(static_cast<int>(m_moveAreaRect.right / 4),
												static_cast<int>(m_moveAreaRect.bottom / 4));
				CPoint p2(static_cast<int>(m_moveAreaRect.right / 2 + m_moveAreaRect.right / 4),
												static_cast<int>(m_moveAreaRect.bottom / 4));
				CPoint p3(static_cast<int>(m_moveAreaRect.right / 2),
												static_cast<int>(m_moveAreaRect.bottom / 2 + m_moveAreaRect.bottom / 4));

				m_vectShapes.emplace_back( ShapesFactory(shape_type, p1) );
				m_vectShapes.emplace_back( ShapesFactory(shape_type, p2) );
				m_vectShapes.emplace_back( ShapesFactory(shape_type, p3) );
			}
			break;
		case 4:
		default:
			{
				CPoint p1(static_cast<int>(m_moveAreaRect.right / 4),
										static_cast<int>(m_moveAreaRect.bottom / 4));
				CPoint p2(static_cast<int>(m_moveAreaRect.right / 2 + m_moveAreaRect.right / 4),
										static_cast<int>(m_moveAreaRect.bottom / 4));
				CPoint p3(static_cast<int>(m_moveAreaRect.right / 4),
										static_cast<int>(m_moveAreaRect.bottom / 2 + m_moveAreaRect.bottom / 4));
				CPoint p4(static_cast<int>(m_moveAreaRect.right / 2 + m_moveAreaRect.right / 4),
										static_cast<int>(m_moveAreaRect.bottom / 2 + m_moveAreaRect.bottom / 4));

				m_vectShapes.emplace_back( ShapesFactory(shape_type, p1) );
				m_vectShapes.emplace_back( ShapesFactory(shape_type, p2) );
				m_vectShapes.emplace_back( ShapesFactory(shape_type, p3) );
				m_vectShapes.emplace_back( ShapesFactory(shape_type, p4) );
			}
			break;
	}
}

void CShapesManager::SetDCpointer(CDC* pdc)
{
	for (auto &shape : m_vectShapes)
	{
		shape->SetDCpointer(pdc);
	}
}

void CShapesManager::SetShapesWidth(short width)
{
	if (m_vectShapes.empty())
		return;

	for (auto it = m_vectShapes.begin();
			it != m_vectShapes.end(); ++it)
	{
		(*it)->SetWidth(width);
	}
}

void CShapesManager::SetShapesHeight(short height)
{
	if (m_vectShapes.empty())
		return;

	for (auto it = m_vectShapes.begin();
			it != m_vectShapes.end(); ++it)
	{
		(*it)->SetHeight(height);
	}
}

void CShapesManager::SetShapesSpeed(short speed)
{
	if (m_vectShapes.empty())
		return;

	for (auto it = m_vectShapes.begin();
		it != m_vectShapes.end(); ++it)
	{
		(*it)->SetSpeed(speed);
	}
}

void CShapesManager::SetShapesMoveArea(CRect rc)
{
	if (m_vectShapes.empty())
		return;

	for (auto it = m_vectShapes.begin();
		it != m_vectShapes.end(); ++it)
	{
		(*it)->SetMoveArea(rc);
	}
}

void CShapesManager::RedrawShapes(HWND hWnd)
{
	for (auto it = m_vectShapes.begin();
		it != m_vectShapes.end(); ++it)
	{
		(*it)->RedrawShapes(hWnd);
	}
}

void CShapesManager::SetShapesColor(COLORREF color)
{
	if (m_vectShapes.empty())
		return;

	for (auto it = m_vectShapes.begin();
		it != m_vectShapes.end(); ++it)
	{
		(*it)->SetColor(color);
	}
}

short CShapesManager::GetShapesWidth() const
{
	if (m_vectShapes.empty())
		return 0;

	return m_vectShapes.at(0)->GetWidth();
}

short CShapesManager::GetShapesHeight() const
{
	if (m_vectShapes.empty())
		return 0;

	return m_vectShapes.at(0)->GetHeight();
}

short CShapesManager::GetShapesSpeed() const
{
	if (m_vectShapes.empty())
		return 0;

	return m_vectShapes.at(0)->GetSpeed();
}

COLORREF CShapesManager::GetShapesColor() const
{
	if (m_vectShapes.empty())
		return 0;

	return m_vectShapes.at(0)->GetColor();
}

void CShapesManager::ComputeNewPositions(CRect areaRect)
{
	if (m_vectShapes.empty())
		return;

	auto shapes_count = m_vectShapes.size();
	assert(shapes_count > 0 && shapes_count <= 4);

	switch (shapes_count)
	{
		case 1:
		{
			m_vectShapes.at(0)->SetNewPos(
								static_cast<int>(areaRect.right / 2), 
								static_cast<int>(areaRect.bottom / 2));
		}
		break;
		case 2:
		{
			m_vectShapes.at(0)->SetNewPos(
								static_cast<int>(areaRect.right / 4),
								static_cast<int>(areaRect.bottom / 2));
			m_vectShapes.at(1)->SetNewPos(
								static_cast<int>(areaRect.right / 2 + areaRect.right / 4),
								static_cast<int>(areaRect.bottom / 2));
		}
		break;
		case 3:
		{
			m_vectShapes.at(0)->SetNewPos(
								static_cast<int>(areaRect.right / 4),
								static_cast<int>(areaRect.bottom / 4));
			m_vectShapes.at(1)->SetNewPos(
								static_cast<int>(areaRect.right / 2 + areaRect.right / 4),
								static_cast<int>(areaRect.bottom / 4));
			m_vectShapes.at(2)->SetNewPos(
								static_cast<int>(areaRect.right / 2),
								static_cast<int>(areaRect.bottom / 2 + areaRect.bottom / 4));
		}
		break;
		case 4:
		default:
		{
			m_vectShapes.at(0)->SetNewPos(
								static_cast<int>(areaRect.right / 4),
								static_cast<int>(areaRect.bottom / 4));
			m_vectShapes.at(1)->SetNewPos(
								static_cast<int>(areaRect.right / 2 + areaRect.right / 4),
								static_cast<int>(areaRect.bottom / 4));
			m_vectShapes.at(2)->SetNewPos(
								static_cast<int>(areaRect.right / 4),
								static_cast<int>(areaRect.bottom / 2 + areaRect.bottom / 4));
			m_vectShapes.at(3)->SetNewPos(
								static_cast<int>(areaRect.right / 2 + areaRect.right / 4),
								static_cast<int>(areaRect.bottom / 2 + areaRect.bottom / 4));
		}
		break;
	}
}

void CShapesManager::Draw()
{
	for (auto &shape : m_vectShapes)
	{
		shape->Draw();
	}
}

void CShapesManager::MoveShapes()
{
	for (auto &shape : m_vectShapes)
	{
		shape->Move();
	}
}

bool CShapesManager::isPointInRectangle(CPoint point)
{
	for (auto &shape : m_vectShapes)
	{
		if (shape->GetRect().PtInRect(point))
			return true;
	}
	return false;
}

void CShapesManager::RearangeShapes()
{
	ComputeNewPositions(m_moveAreaRect);
}
