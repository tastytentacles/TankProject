class _Point2D
{
public:
	float _x;
	float _y;
};

class _BB_RoteAble
{
public:
	_BB_RoteAble(float $width, float $height);

	bool _Get_PointIntersect(float $posX, float $posY, float $pointX, float $pointY, float $direction);

	float _width;
	float _height;
};

class _Orbital
{
public:
	_Orbital();

	_Point2D _pos;

	float _Direction;
	float _Speed;

	float _faceVertexArray;
};

struct _Key_ID
{
	char _UP_ID;
	char _DOWN_ID;
	char _LEFT_ID;
	char _RIGHT_ID;

	char _TRot_Right_ID;
	char _TRot_Left_ID;
};

class _Tank : public _Orbital
{
public:
	_Tank();

	_BB_RoteAble _hitBox;

	_Key_ID _Tank_Key_ID;

	float _Turret_Direction;

	bool _Key_Up;
	bool _Key_Down;
	bool _Key_Left;
	bool _Key_Right;

	bool _Key_TRot_Right;
	bool _Key_TRot_Left;
};

class _Shell : public _Orbital
{
	
};