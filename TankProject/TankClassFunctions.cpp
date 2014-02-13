#include "TankClasses.h"

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
	_Key_Up = false;
	_Key_Down = false;
	_Key_Left = false;
	_Key_Right = false;

	_Key_TRot_Right = false;
	_Key_TRot_Left = false;

	_Turret_Direction = 0.0;
}