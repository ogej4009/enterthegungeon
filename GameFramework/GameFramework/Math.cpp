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
	// �� ������ �Ÿ��� ���Ѵ�. �� �Ÿ��� �����ﰢ���� ������
	// ���̰� �ȴ�.
	float	fDist = tSrc.Distance(tDest);

	// �غ��� ���̸� ���Ѵ�.
	float	fW = tSrc.x - tDest.x;

	// cos ��Ÿ���� ���Ѵ�.
	float	fAngle = fW / fDist;

	// ���Լ��� �̿��� ���Ȱ��� �����.
	fAngle = acosf(fAngle);

	// �Ϲ� ������ ������ش�.
	fAngle = RadianToAngle(fAngle);

	// DEST�� Y��ǥ -> SRC Y��ǥ 
	// (-) �����̽��� ���� ������ ���Ѵ�.  
	if (tSrc.y < tDest.y)
		fAngle = 360.f - fAngle;

	return fAngle;
}
