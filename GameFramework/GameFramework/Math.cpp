#include "Math.h"
#include "Game.h"
#include "Timer.h"

//CVec CVec::DLeft;
//CVec CVec::DRight;
//CVec CVec::DTop;
//CVec CVec::DBottom;
//
//const CVec CVec::ZERO = { 0,  0 };
//const CVec CVec::LEFT = { -1,  0 };
//const CVec CVec::LEFTDOWN = { -1,  1 };
//const CVec CVec::DOWN = { 0 ,  1 };
//const CVec CVec::DOWNRIGHT = { 1 ,  1 };
//const CVec CVec::RIGHT = { 1 ,  0 };
//const CVec CVec::RIGHTUP = { 1 , -1 };
//const CVec CVec::UP = { 0 , -1 };
//const CVec CVec::UPLEFT = { -1, -1 };
//
//const CVec CVec::ARRDIR[8] = { CVec::LEFT, CVec::LEFTDOWN, CVec::DOWN, CVec::DOWNRIGHT
//									 , CVec::RIGHT, CVec::RIGHTUP, CVec::UP, CVec::UPLEFT };
//
//CVec CVec::DeltaDirArr[MD_MAX];
//
//
//void CVec::DirUpdate()
//{
//	DeltaDirArr[L] = DLeft = LEFT * CTimer::DeltaTime();
//	DeltaDirArr[R] = DRight = RIGHT * CTCTimerime::DeltaTime();
//	DeltaDirArr[T] = DTop = UP * CTimer::DeltaTime();
//	DeltaDirArr[B] = DBottom = DOWN * CTimer::DeltaTime();
//}
//
//const CVec& CVec::DeltaDir(MONSTER_DIR dir)
//{
//	return DeltaDirArr[dir];
//}



/////////////////////////////////////////////////////////////////////////////////////////////////

CMath::CMath()
{
}

CMath::~CMath()
{
}
float CMath::RadianToAngle(float fRadian)
{
	return fRadian * 180.f / PI;
}

float CMath::AngleToRadian(float fAngle)
{
	return fAngle * PI / 180.f;
}

float CMath::GetAngle(const _tagPosition& tSrc,
	const _tagPosition& tDest)
{
	// 두 점간의 거리를 구한다. 이 거리가 직각삼각형의 빗변의
	// 길이가 된다.
	float	fDist = tSrc.Distance(tDest);

	// 밑변의 길이를 구한다.
	float	fW = tSrc.x - tDest.x;

	// cos 세타값을 구한다.
	float	fAngle = fW / fDist;

	// 역함수를 이용해 라디안값을 만든다.
	fAngle = acosf(fAngle);

	// 일반 각도로 만들어준다.
	fAngle = RadianToAngle(fAngle);

	// DEST의 Y좌표 -> SRC Y좌표 
	// (-) 스페이스에 속한 각도를 구한다.  
	if (tSrc.y < tDest.y)
		fAngle = 360.f - fAngle;

	return fAngle;
}
