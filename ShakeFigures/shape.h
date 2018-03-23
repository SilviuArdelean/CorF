#pragma once
#include "stdafx.h"

class CShape
{
  public:
	CShape(CPoint centerPoint, 
			short width, short height, short speed, CRect allowedMoveArea)
		: _centerPoint(centerPoint)
		, _width(width)
		, _height(height)
		, _speed(speed)
		, _allowedMoveAreaRect(allowedMoveArea)
	{	
		_color = GetRandomColor();
		_moveX = _moveY = _speed;
		CalculateShapeRect();
	}

	CShape(short centerPointX, short centerPointY, 
			short width, short height, short speed, CRect allowedMoveArea)
		: _width(width)
		, _height(height)
		, _speed(speed)
		, _allowedMoveAreaRect(allowedMoveArea)
	{
		_color = GetRandomColor();
		_centerPoint.x = centerPointX;
		_centerPoint.x = centerPointY;

		_moveX = _moveY = _speed;
		CalculateShapeRect();
	}

	CShape(const CShape& rhs)
	{
		_centerPoint = rhs._centerPoint;
		_rect = rhs._rect;
		_width = rhs._width;
		_height = rhs._height;
		_speed = rhs._speed;
		_allowedMoveAreaRect = rhs._allowedMoveAreaRect;
		_moveX = _moveY = _speed;
		_color = rhs._color;
	}

	CShape& operator = (const CShape& rhs)
	{
		if (&rhs != this)
		{
			_centerPoint = rhs._centerPoint;
			_rect = rhs._rect;
			_width = rhs._width;
			_height = rhs._height;
			_speed = rhs._speed;
			_allowedMoveAreaRect = rhs._allowedMoveAreaRect;
			_moveX = _moveY = _speed;
			_color = rhs._color;
		}

		return *this;
	}

	CShape(CShape&& rhs)
	{
		_centerPoint = rhs._centerPoint;
		_rect = rhs._rect;
		_width = rhs._width;
		_height = rhs._height;
		_speed = rhs._speed;
		_allowedMoveAreaRect = rhs._allowedMoveAreaRect;
		_moveX = _moveY = _speed;
		_color = rhs._color;

		rhs._centerPoint = 0;
		rhs._rect = 0;
		rhs._width = 0;
		rhs._height = 0;
		rhs._speed = 0;
		rhs._allowedMoveAreaRect = 0;
		rhs._color = 0;
	}

	CShape& operator = (CShape&& rhs)
	{
		if (&rhs != this)
		{
			_centerPoint = rhs._centerPoint;
			_rect = rhs._rect;
			_width = rhs._width;
			_height = rhs._height;
			_speed = rhs._speed;
			_allowedMoveAreaRect = rhs._allowedMoveAreaRect;
			_moveX = _moveY = _speed;
			_color = rhs._color;

			rhs._centerPoint = 0;
			rhs._rect = 0; 
			rhs._width = 0;
			rhs._height = 0;
			rhs._speed = 0;
			rhs._allowedMoveAreaRect = 0;
			rhs._color = 0;
		}

		return *this;
	}

	virtual ~CShape() {}

  public:
	void SetNewPos(int x, int y)
	{
		_centerPoint.x = x;
		_centerPoint.y = y;

		CalculateShapeRect();
	}

	void SetNewPos(CPoint point)
	{
		_centerPoint = point;

		CalculateShapeRect();
	}

	void	SetWidth(short width) { _width = width; CalculateShapeRect(); }
	void	SetHeight(short height) { _height = height; CalculateShapeRect(); }
	void	SetSpeed(short speed) { _speed = speed; }

	int		GetWidth() const { return _width; }
	int		GetHeight() const { return _height; }
	int		GetSpeed() const { return _speed; }

	void	SetColor(COLORREF color) { _color = color; }
	COLORREF  GetColor() const { return _color; }

	void SetMoveArea(CRect rc) { _allowedMoveAreaRect = rc; }
	void SetDCpointer(CDC* pdc) { _pDC = pdc; }

	CRect& GetRect() { return _rect; }

	virtual void Draw() = 0;

	void RedrawShapes(HWND hWnd) { InvalidateRect(hWnd, _rect, TRUE); };

	void Move()
	{
		_centerPoint.x += _moveX;
		_centerPoint.y += _moveY;

		if (_centerPoint.x > _allowedMoveAreaRect.right)
			_moveX = -_moveX;

		if (_centerPoint.y > _allowedMoveAreaRect.bottom)
			_moveY = -_moveY;

		if (_centerPoint.y < _allowedMoveAreaRect.top)
			_moveY = -_moveY;

		if (_centerPoint.x < _allowedMoveAreaRect.left)
			_moveX = -_moveX;


		CalculateShapeRect();

		_rect.MoveToXY(_centerPoint);
	}



  protected:
	CPoint		_centerPoint;
	CRect		_rect;
	int			_width;
	int			_height;
	int			_speed;
	CDC*		_pDC;
	CRect		_allowedMoveAreaRect;
	
	int			_moveX;
	int			_moveY;
	COLORREF	_color;

  private:
	std::vector<COLORREF>  _colorsArray = { RGB(255, 0, 0), RGB(0, 255, 0),RGB(0, 0, 255) };
	COLORREF GetRandomColor()		// returns a random specific only color
	{
		srand((unsigned int)time(0));
		auto color_index = rand() % _colorsArray.size();
		auto color = _colorsArray.at(color_index);
		return color;
	}

    void CalculateShapeRect()
	{
		  _rect.left = _centerPoint.x - _width / 2;
		  _rect.top = _centerPoint.y - _height / 2;
		  _rect.right = _centerPoint.x + _width / 2;
		  _rect.bottom = _centerPoint.y + _height / 2;
	}

};

