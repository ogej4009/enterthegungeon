#include "Collider.h"
#include "CollisionManager.h"
#include "ColliderRect.h"
#include "ColliderSphere.h"
#include "ColliderPoint.h"
#include "GameManager.h"

CCollider::CCollider() :
	m_pObj(nullptr),
	m_pProfile(nullptr),
	m_iChannelIndex(0),
	m_bCollision(false),
	m_bIsUI(false),
	m_iSerialNumber(UINT_MAX)
{
}

CCollider::CCollider(const CCollider& collider)
{
	*this = collider;
	m_pObj = nullptr;
	m_bCollision = false;
}

CCollider::~CCollider()
{
	if (m_iSerialNumber != UINT_MAX)
	{
		float	fTime = GET_SINGLE(CGameManager)->GetDeltaTime();

		list<CCollider*>::iterator	iter;
		list<CCollider*>::iterator	iterEnd = m_PrevColliderList.end();

		for (iter = m_PrevColliderList.begin(); iter != iterEnd; ++iter)
		{
			// 인접행렬에서 기존에 충돌이 되고 있다면 모두 제거해준다.
			if (GET_SINGLE(CCollisionManager)->CheckPrevCollisionAdj(m_iSerialNumber,
				(*iter)->GetSerialNumber()))// 시리얼넘버를 지운다. 
			{
				GET_SINGLE(CCollisionManager)->DeletePrevCollisionAdj(m_iSerialNumber,
					(*iter)->GetSerialNumber());// 시리얼넘버를 지운다. 

				(*iter)->DeletePrevCollider(this);// 충돌체자체를 지운다. 

				Call(CS_RELEASE, *iter, fTime);
				(*iter)->Call(CS_RELEASE, this, fTime);
			}
		}
		// 충돌매니저가 시리얼넘버를 지정한다.
		GET_SINGLE(CCollisionManager)->ReturnSerialNumber(m_iSerialNumber);
	}
}

void CCollider::SetProfile(const string& strName)
{
	/*
		프로파일 할 때마다 강제로 바꾸면 프로파일 디폴트 강제 세팅 내가 쓸 채널 한개가 무시되어버림
		충돌 매니저에서 프로파일의 이름을 가지고
		디폴트 채널 인덱스를 가지고 있음
		내가 만약 채널을 먼저 세팅을 해서 먼저 채널을 찾아서 그다음 setprofile함수 호출하면,
		이 프로파일이 디폴트 채널인덱스를 가지고 있으므로 내가 먼저 호출한 SetProfile채널이 완전히 갱신된다 . 덮어쓴다.
	*/
	m_pProfile = GET_SINGLE(CCollisionManager)->FindProfile(strName);
}

void CCollider::SetChannel(const string& strName)
{
	/*
		채널을 지정한다는 것은 동적배열로 등록한 채널들의 인덱스번호를 가져와서 어떤 번호인지 현재 충돌체에 지정을 해주는 것이다.
	*/
	m_iChannelIndex = GET_SINGLE(CCollisionManager)->GetChannelIndex(strName);
}

void CCollider::AddPrevCollider(CCollider* pCollider)
{
	m_PrevColliderList.push_back(pCollider);
}

void CCollider::DeletePrevCollider(CCollider* pCollider)
{
	list<CCollider*>::iterator	iter;
	list<CCollider*>::iterator	iterEnd = m_PrevColliderList.end();

	for (iter = m_PrevColliderList.begin(); iter != iterEnd; ++iter)
	{
		if (*iter == pCollider)
		{
			m_PrevColliderList.erase(iter);
			break;
		}
	}
}

void CCollider::Start()
{
	/*
		Obj의 Start에서 충돌체 객체들을 하나씩 호출한다.
		충돌체 매니져는 시리얼넘버를 부여한다.
		충돌체 매니져는 이 충돌체객체를 등록한다. 즉 각각의 충돌체들은 자신을 충돌체 매니져에 등록한다.
	*/
	// 충돌체 관리자는 시리얼넘버를 부여한다. 
	m_iSerialNumber = GET_SINGLE(CCollisionManager)->CreateSerialNumber();
	// 자기자신을 충돌체로 등록한다. 충돌체 관리자는 이 충돌체를 리스트로 관리한다. 
	GET_SINGLE(CCollisionManager)->AddCollider(this);
}

bool CCollider::Init()
{
	// 프로파일과 채널 둘다 init에서 가지고 있을 수 있게 만든다. 
	// 충돌매니저의 FindProfile
	m_pProfile = GET_SINGLE(CCollisionManager)->FindProfile("BlockAll");
	// GetChannelIndex은 채널의 인덱스를 부여한다.
	m_iChannelIndex = GET_SINGLE(CCollisionManager)->GetChannelIndex("WorldStatic");

	return true;
}

void CCollider::Update(float fTime)
{
}

void CCollider::LateUpdate(float fTime)
{
}

void CCollider::PrevRender(HDC hDC, float fTime)
{
}

void CCollider::Render(HDC hDC, float fTime)
{
}

void CCollider::PostRender(HDC hDC, float fTime)
{
}

CCollider* CCollider::Clone() const
{
	return nullptr;
}

void CCollider::Save(FILE* pFile)
{
	int	iLength = m_strTag.length();

	fwrite(&iLength, sizeof(int), 1, pFile);
	fwrite(m_strTag.c_str(), sizeof(char), iLength, pFile);

	fwrite(&m_eColliderType, sizeof(COLLIDER_TYPE), 1, pFile);
	fwrite(&m_bCollision, sizeof(bool), 1, pFile);
	fwrite(&m_bIsUI, sizeof(bool), 1, pFile);

	iLength = m_pProfile->strName.length();
	fwrite(&iLength, sizeof(int), 1, pFile);
	fwrite(m_pProfile->strName.c_str(), sizeof(char), iLength, pFile);

	fwrite(&m_iChannelIndex, sizeof(int), 1, pFile);
}

void CCollider::Load(FILE* pFile)
{
	int	iLength = 0;

	fread(&iLength, sizeof(int), 1, pFile);
	char	strText[256] = {};
	fread(strText, sizeof(char), iLength, pFile);
	m_strTag = strText;

	fread(&m_eColliderType, sizeof(COLLIDER_TYPE), 1, pFile);
	fread(&m_bCollision, sizeof(bool), 1, pFile);
	fread(&m_bIsUI, sizeof(bool), 1, pFile);

	fread(&iLength, sizeof(int), 1, pFile);
	memset(strText, 0, 256);
	fread(strText, sizeof(char), iLength, pFile);

	SetProfile(strText);

	fread(&m_iChannelIndex, sizeof(int), 1, pFile);
}

void CCollider::UpdateObjPos(const Position& tPos)
{
	/*
		오브젝트 위치정보를 갱신할 때
		충돌체들의 위치에까지 영향을 주게 만들어준 함수
	*/
}

bool CCollider::CollisionRectToRect(CCollider* pSrc,
	CCollider* pDest)
{
	Position	tIntersect; // 교점 

	if (CollisionRectToRect(((CColliderRect*)pSrc)->GetInfo(),
		((CColliderRect*)pDest)->GetInfo(), tIntersect))
	{
		// 교점을 만든다. 
		pSrc->m_tIntersect = tIntersect;
		pDest->m_tIntersect = tIntersect;

		pSrc->m_bCollision = true;
		pDest->m_bCollision = true;

		// 충돌 되었다. 
		return true;
	}

	return false;
}

bool CCollider::CollisionSphereToSphere(CCollider* pSrc, CCollider* pDest)
{
	Position	tIntersect;

	if (CollisionSphereToSphere(((CColliderSphere*)pSrc)->GetInfo(),
		((CColliderSphere*)pDest)->GetInfo(), tIntersect))
	{
		pSrc->m_tIntersect = tIntersect;
		pDest->m_tIntersect = tIntersect;

		pSrc->m_bCollision = true;
		pDest->m_bCollision = true;

		return true;
	}

	return false;
}

bool CCollider::CollisionRectToSphere(CCollider* pSrc, CCollider* pDest)
{
	Position	tIntersect;

	if (CollisionRectToSphere(((CColliderRect*)pSrc)->GetInfo(),
		((CColliderSphere*)pDest)->GetInfo(), tIntersect))
	{
		pSrc->m_tIntersect = tIntersect;
		pDest->m_tIntersect = tIntersect;

		pSrc->m_bCollision = true;
		pDest->m_bCollision = true;

		return true;
	}

	return false;
}

bool CCollider::CollisionRectToPoint(CCollider* pSrc, CCollider* pDest)
{
	Position	tIntersect;

	if (CollisionRectToPoint(((CColliderRect*)pSrc)->GetInfo(),
		((CColliderPoint*)pDest)->GetInfo(), tIntersect))
	{
		pSrc->m_tIntersect = tIntersect;
		pDest->m_tIntersect = tIntersect;

		pSrc->m_bCollision = true;
		pDest->m_bCollision = true;

		return true;
	}

	return false;
}

bool CCollider::CollisionSphereToPoint(CCollider* pSrc, CCollider* pDest)
{
	Position	tIntersect;

	if (CollisionSphereToPoint(((CColliderSphere*)pSrc)->GetInfo(),
		((CColliderPoint*)pDest)->GetInfo(), tIntersect))
	{
		pSrc->m_tIntersect = tIntersect;
		pDest->m_tIntersect = tIntersect;

		pSrc->m_bCollision = true;
		pDest->m_bCollision = true;

		return true;
	}

	return false;
}

bool CCollider::CollisionPointToPoint(CCollider* pSrc, CCollider* pDest)
{
	Position	tIntersect;

	if (CollisionPointToPoint(((CColliderPoint*)pSrc)->GetInfo(),
		((CColliderPoint*)pDest)->GetInfo(), tIntersect))
	{
		pSrc->m_tIntersect = tIntersect;
		pDest->m_tIntersect = tIntersect;

		pSrc->m_bCollision = true;
		pDest->m_bCollision = true;

		return true;
	}

	return false;
}

bool CCollider::CollisionRectToRect(const RectInfo& tSrc,
	const RectInfo& tDest, Position& tIntersect)
{
	if (tSrc.l > tDest.r)
		return false;

	else if (tSrc.r < tDest.l)
		return false;

	else if (tSrc.t > tDest.b)
		return false;

	else if (tSrc.b < tDest.t)
		return false;

	float	l, t, r, b;
	// rect 모서리 l, t, r, b			  조건식	    	true	false
	// 삼항연산자 : if분기문 간략하게  tSrc.l > tDest.l ? tSrc.l : tDest.l;
	l = tSrc.l > tDest.l ? tSrc.l : tDest.l;
	r = tSrc.r < tDest.r ? tSrc.r : tDest.r;
	t = tSrc.t > tDest.t ? tSrc.t : tDest.t;
	b = tSrc.b < tDest.b ? tSrc.b : tDest.b;

	tIntersect.x = (l + r) / 2.f;
	tIntersect.y = (t + b) / 2.f;


	return true;
}

bool CCollider::CollisionSphereToSphere(const SphereInfo& tSrc,
	const SphereInfo& tDest, Position& tIntersect)
{
	// 두 원의 중점간의 거리를 구한다.
	float	fDist = tSrc.tCenter.Distance(tDest.tCenter);

	if (fDist > tSrc.fRadius + tDest.fRadius)
		return false;

	// Src의 중점에서 Dest의 중점으로 향할 경우 Src의 중점에서 겹치지 않은
	// 부분의 길이를 구한다.
	float	fLength = fDist - tDest.fRadius;

	if (fLength <= 0.f)
	{
		tIntersect = tSrc.tCenter;
		return true;
	}

	fLength = fDist - tSrc.fRadius;

	if (fLength <= 0.f)
	{
		tIntersect = tDest.tCenter;
		return true;
	}

	// 겹치는 구간의 길이를 구한다.
	float	fIntersectLength = (tSrc.fRadius + tDest.fRadius) - fDist;

	fLength = fDist - tDest.fRadius + fIntersectLength / 2.f;

	float	fXDir = 0.f;
	float	fYDir = 0.f;

	if (tDest.tCenter.x - tSrc.tCenter.x > 0.f)
		fXDir = 1.f;

	else if (tDest.tCenter.x - tSrc.tCenter.x < 0.f)
		fXDir = -1.f;

	if (tDest.tCenter.y - tSrc.tCenter.y > 0.f)
		fYDir = 1.f;

	else if (tDest.tCenter.y - tSrc.tCenter.y < 0.f)
		fYDir = -1.f;

	// 두점간의 각도를 구한다.
	float	fAngle = CMath::GetAngle(tSrc.tCenter, tDest.tCenter);

	tIntersect.x = tSrc.tCenter.x + cosf(CMath::AngleToRadian(fAngle)) * fLength;
	tIntersect.y = tSrc.tCenter.y + sinf(CMath::AngleToRadian(fAngle)) * fLength;

	return true;
}

bool CCollider::CollisionRectToSphere(const RectInfo& tSrc,
	const SphereInfo& tDest, Position& tIntersect)
{
	// 원의 중점이 사각형의 좌, 우 안에 포함되거나
	// 사각형의 위, 아래 안에 포함될 경우 사각형을 원의 반지름만큼
	// 확장해서 중점이 확장된 사각형에 들어오는지 판단한다.
	if ((tSrc.l <= tDest.tCenter.x && tDest.tCenter.x <= tSrc.r) ||
		(tSrc.t <= tDest.tCenter.y && tDest.tCenter.y <= tSrc.b))
	{
		// 반지름만큼 확장된 사각형을 만든다.
		RectInfo	tRC;
		tRC.l = tSrc.l - tDest.fRadius;
		tRC.t = tSrc.t - tDest.fRadius;
		tRC.r = tSrc.r + tDest.fRadius;
		tRC.b = tSrc.b + tDest.fRadius;

		if (tDest.tCenter.x < tRC.l)
			return false;

		else if (tDest.tCenter.x > tRC.r)
			return false;

		else if (tDest.tCenter.y < tRC.t)
			return false;

		else if (tDest.tCenter.y > tRC.b)
			return false;

		tRC.l = tDest.tCenter.x - tDest.fRadius;
		tRC.t = tDest.tCenter.y - tDest.fRadius;
		tRC.r = tDest.tCenter.x + tDest.fRadius;
		tRC.b = tDest.tCenter.y + tDest.fRadius;

		float	l, t, r, b;
		// rect 모서리 l, t, r, b			  조건식	    	true	false
		// 삼항연산자 : if분기문 간략하게  tSrc.l > tDest.l ? tSrc.l : tDest.l;
		l = tSrc.l > tRC.l ? tSrc.l : tRC.l;
		r = tSrc.r < tRC.r ? tSrc.r : tRC.r;
		t = tSrc.t > tRC.t ? tSrc.t : tRC.t;
		b = tSrc.b < tRC.b ? tSrc.b : tRC.b;

		tIntersect.x = (l + r) / 2.f;
		tIntersect.y = (t + b) / 2.f;

		return true;
	}

	// 사각형을 구성하는 4개의 점중 원 안에 하나라도 들어간다면
	// 충돌이다.
	Position	tPos[4];
	tPos[0] = Position(tSrc.l, tSrc.t);
	tPos[1] = Position(tSrc.r, tSrc.t);
	tPos[2] = Position(tSrc.l, tSrc.b);
	tPos[3] = Position(tSrc.r, tSrc.b);

	for (int i = 0; i < 4; ++i)
	{
		if (tPos[i].Distance(tDest.tCenter) <= tDest.fRadius)
		{
			RectInfo	tRC;

			tRC.l = tDest.tCenter.x - tDest.fRadius;
			tRC.t = tDest.tCenter.y - tDest.fRadius;
			tRC.r = tDest.tCenter.x + tDest.fRadius;
			tRC.b = tDest.tCenter.y + tDest.fRadius;

			float	l, t, r, b;

			// rect 모서리 l, t, r, b			  조건식	    	true	false
			// 삼항연산자 : if분기문 간략하게  tSrc.l > tDest.l ? tSrc.l : tDest.l;
			l = tSrc.l > tRC.l ? tSrc.l : tRC.l;
			r = tSrc.r < tRC.r ? tSrc.r : tRC.r;
			t = tSrc.t > tRC.t ? tSrc.t : tRC.t;
			b = tSrc.b < tRC.b ? tSrc.b : tRC.b;

			tIntersect.x = (l + r) / 2.f;
			tIntersect.y = (t + b) / 2.f;

			return true;

		}
	}

	return false;
}

bool CCollider::CollisionRectToPoint(const RectInfo& tSrc,
	const Position& tDest, Position& tIntersect)
{
	if (tSrc.l > tDest.x)
		return false;

	else if (tSrc.t > tDest.y)
		return false;

	else if (tSrc.r < tDest.x)
		return false;

	else if (tSrc.b < tDest.y)
		return false;

	tIntersect = tDest;

	return true;
}

bool CCollider::CollisionSphereToPoint(const SphereInfo& tSrc,
	const Position& tDest, Position& tIntersect)
{
	float	fDist = tSrc.tCenter.Distance(tDest);

	if (fDist > tSrc.fRadius)
		return false;

	tIntersect = tDest;

	return true;
}

bool CCollider::CollisionPointToPoint(const Position& tSrc,
	const Position& tDest, Position& tIntersect)
{
	if ((int)tSrc.x != (int)tDest.x || (int)tSrc.y != (int)tDest.y)
		return false;

	tIntersect = tDest;

	return true;
}
