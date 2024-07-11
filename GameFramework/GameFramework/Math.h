#pragma once
#include <math.h>



#define	PI		3.141592f
#define	GRAVITY	9.8f

//class CVec
//{
//public:
//	static void DirUpdate();
//
//	CVec() : X(0.0f), Y(0.0f) {}
//	CVec(float x, float y) : X(x), Y(y) {}
//	CVec(int x, int y) : X((float)x), Y((float)y) {}
//
//	float HalfX() const { return X * 0.5f; }
//	float HalfY() const { return Y * 0.5f; }
//
//	static const CVec& DeltaLeft() { return DLeft; }
//	static const CVec& DeltaRight() { return DRight; }
//	static const CVec& DeltaTop() { return DTop; }
//	static const CVec& DeltaBottom() { return DBottom; }
//	static const CVec& DeltaDir(MONSTER_DIR dir);
//
//public:
//	static const CVec CZero;
//
//	static const CVec CLeft;
//	static const CVec LEFTDOWN;
//	static const CVec DOWN;
//	static const CVec DOWNRIGHT;
//	static const CVec RIGHT;
//	static const CVec RIGHTUP;
//	static const CVec UP;
//	static const CVec UPLEFT;
//
//	static const CVec ARRDIR[8];
//
//	float X;
//	float Y;
//
//private:
//	static CVec DLeft;
//	static CVec DRight;
//	static CVec DTop;
//	static CVec DBottom;
//
//	static CVec DeltaDirArr[MD_MAX];
//
//	/* operator overloading */
//public:
//	bool operator==(const CVec& _Pos) const
//	{
//		return X == _Pos.X && Y == _Pos.Y;
//	}
//
//	bool operator!=(const CVec& _Pos) const
//	{
//		return X != _Pos.X || Y != _Pos.Y;
//	}
//
//	bool operator>=(const CVec& _Pos) const
//	{
//		return X >= _Pos.X && Y >= _Pos.Y;
//	}
//
//	bool operator<=(const CVec& _Pos) const
//	{
//		return X <= _Pos.X && Y <= _Pos.Y;
//	}
//
//	CVec operator *(const float _Value) const
//	{
//		return { this->X * _Value, this->Y * _Value };
//	}
//
//	CVec operator /(const float _Value) const
//	{
//		return { this->X / _Value, this->Y / _Value };
//	}
//
//	CVec& operator *=(const float _Value)
//	{
//		this->X *= _Value;
//		this->Y *= _Value;
//		return *this;
//	}
//
//	CVec operator +(const CVec& other) const
//	{
//		return { this->X + other.X, this->Y + other.Y };
//	}
//
//	CVec operator +(const int other) const
//	{
//		return { this->X + other, this->Y + other };
//	}
//
//	CVec& operator +=(const CVec& other)
//	{
//		this->X += other.X;
//		this->Y += other.Y;
//		return *this;
//	}
//
//	CVec operator -(const CVec& other) const
//	{
//		return { this->X - other.X, this->Y - other.Y };
//	}
//
//	CVec& operator -=(const CVec& other)
//	{
//		this->X -= other.X;
//		this->Y -= other.Y;
//		return *this;
//	}
//
//	/* normal Fuction */
//	inline int IntX() const { return (int)X; }
//	inline int IntY() const { return (int)Y; }
//	inline int IntHalfX() const { return (int)HalfX(); }
//	inline int IntHalfY() const { return (int)HalfY(); }
//	inline float Length() const { return sqrtf((X * X) + (Y * Y)); }
//
//
//}; 


class CMath
{
public:
	CMath();
	~CMath();

public:
	static float RadianToAngle(float fRadian);
	static float AngleToRadian(float fAngle);
	static float GetAngle(const struct _tagPosition& tSrc, const struct _tagPosition& tDest); // 전방선언 
};

