#include "ColliderSphere.h"
#include "Obj.h"
#include "GameManager.h"
#include "Camera.h"

CColliderSphere::CColliderSphere()
{
	// 생성자에서 자신의 충돌 타입이 CT_RECT로 지정되게 한다. 
	m_eColliderType = CT_SPHERE;
	SetTag("Sphere");
}

CColliderSphere::CColliderSphere(const CColliderSphere& collider) :
	CCollider(collider)
{
	m_tRelative = collider.m_tRelative;
	m_tInfo = collider.m_tInfo;
}

CColliderSphere::~CColliderSphere()
{
}

void CColliderSphere::Start()
{
	CCollider::Start();
}

bool CColliderSphere::Init()
{
	return true;
}

void CColliderSphere::Update(float fTime)
{
	m_bCollision = false;
}

void CColliderSphere::LateUpdate(float fTime)
{
	// 반지름(r) : 구의 중점의 위치 + 반지름 
	m_tInfo.tCenter = m_tRelative.tCenter + m_pObj->GetPos();
}

void CColliderSphere::PrevRender(HDC hDC, float fTime)
{
}

void CColliderSphere::Render(HDC hDC, float fTime)
{
#ifdef _DEBUG
	HPEN	hPen = GREENPEN;

	// 충돌하면 '레드'로 변한다
	if (m_bCollision)
		hPen = REDPEN;

	HPEN	hPrevPen = (HPEN)SelectObject(hDC, hPen);

	Position	tLinePos[17]; // 좌표선언 

	// 역함수를 이용해 좌표를 계산한다. 
	for (int i = 0; i < 17; ++i)
	{
		tLinePos[i].x = m_tInfo.tCenter.x + cosf(CMath::AngleToRadian(i * 22.5f)) * m_tInfo.fRadius;
		tLinePos[i].y = m_tInfo.tCenter.y + sinf(CMath::AngleToRadian(i * 22.5f)) * m_tInfo.fRadius;
	}

	Position	tPos = tLinePos[0]; // 시작점 

	// UI는 월드 상의 캐릭터들과는 다르게 카메라의 영역에 속해야 한다. 
	if (!m_bIsUI)
		tPos -= GET_SINGLE(CCamera)->GetPos(); // ＠★ 충돌체도 카메라의 영향을 받는다. 

	MoveToEx(hDC, tPos.x, tPos.y, nullptr);

	// 원 비슷하게 그려준다. 
	for (int i = 1; i < 17; ++i)
	{
		tPos = tLinePos[i];

		// ★ 충돌체도 카메라의 영향을 받는다.  
		// UI는 월드 상의 캐릭터들과는 다르게 카메라의 영역에 속해야 한다. 
		if (!m_bIsUI)
			tPos -= GET_SINGLE(CCamera)->GetPos();

		LineTo(hDC, tPos.x, tPos.y);
	}

	/*
		RECT	tRC = { m_tIntersect.x - 5.f, m_tIntersect.y - 5.f,
		m_tIntersect.x + 5.f, m_tIntersect.y + 5.f };
		FrameRect(hDC, &tRC, REDBRUSH);
	*/

	SelectObject(hDC, hPrevPen);

	//TCHAR	strNumber[32] = {};
	//wsprintf(strNumber, TEXT("SerialNumber : %d"), m_iSerialNumber);
	//TextOut(hDC, m_tInfo.l, m_tInfo.t, strNumber, lstrlen(strNumber));

#endif // _DEBUG

}

void CColliderSphere::PostRender(HDC hDC, float fTime)
{
}

CColliderSphere* CColliderSphere::Clone() const
{
	return new CColliderSphere(*this);
}

bool CColliderSphere::Collision(CCollider* pCollider)
{
	// 충돌체의 모양에 따라 다르게 처리 
	switch (pCollider->GetColliderType())
	{
	case CT_RECT:
		return CollisionRectToSphere(pCollider, this);
	case CT_SPHERE:
		return CollisionSphereToSphere(this, pCollider);
	case CT_POINT:
		return CollisionSphereToPoint(this, pCollider);
	}

	return false;
}

void CColliderSphere::Save(FILE* pFile)
{
	CCollider::Save(pFile);

	fwrite(&m_tRelative, sizeof(SphereInfo), 1, pFile);
}

void CColliderSphere::Load(FILE* pFile)
{
	CCollider::Load(pFile);

	fread(&m_tRelative, sizeof(SphereInfo), 1, pFile);
	m_tInfo.fRadius = m_tRelative.fRadius;
}

void CColliderSphere::UpdateObjPos(const Position& tPos)
{
	/*
		오브젝트 위치정보를 갱신할 때
		충돌체들의 위치에까지 영향을 주게 만들어준 함수
	*/
	// 반지름(r) : 구의 중점의 위치 + 반지름 
	m_tInfo.tCenter = m_tRelative.tCenter + tPos;
}
