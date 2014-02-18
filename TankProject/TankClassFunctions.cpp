#include <math.h>

#include "TankClasses.h"

_BB_RoteAble::_BB_RoteAble(float $width = 1, float $height = 1)
{
	_width = $width;
	_height = $height;
}

bool _BB_RoteAble::_Get_PointIntersect(float $posX, float $posY, float $pointX, float $pointY, float $direction)
{
	float $rotX = cos($direction) * ($pointX - $posX) - sin($direction) * ($pointY - $posY) + $posX;
	float $rotY = sin($direction) * ($pointX - $posX) + cos($direction) * ($pointY - $posY) + $posY;

	if ($rotX < ($posX + (_width / 2)) &&
		$rotX > ($posX - (_width / 2)) &&
		$rotY <($posY + (_height / 2)) &&
		$rotY > ($posY - (_height / 2)))
	{
		return true;
	}
	else
	{
		return false;
	}
}

_Orbital::_Orbital()
{
	//_pos = new _Point2D;

	_pos._x = 0;
	_pos._y = 0;

	_Direction = 0;
	_Speed = 0;
}

_Tank::_Tank()
{	
	_hitBox._height = 1.0f;
	_hitBox._width = 1.0f;
	
	_Key_Up = false;
	_Key_Down = false;
	_Key_Left = false;
	_Key_Right = false;

	_Key_TRot_Right = false;
	_Key_TRot_Left = false;

	_Turret_Direction = 0.0;
}