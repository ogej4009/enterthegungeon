#include "ColliderPoint.h"
#include "Obj.h"
#include "GameManager.h"
#include "Camera.h"


CColliderPoint::CColliderPoint()
{
	// 생성자에서 자신의 충돌 타입이 CT_POINT로 지정되게 한다. 
	m_eColliderType = CT_POINT;
	SetTag("Point");
}

CColliderPoint::CColliderPoint(const CColliderPoint& collider) :
	CCollider(collider)
{
	m_tRelative = collider.m_tRelative;
	m_tInfo = collider.m_tInfo;
}

CColliderPoint::~CColliderPoint()
{
}

void CColliderPoint::Start()
{
	CCollider::Start();
}

bool CColliderPoint::Init()
{
	return true;
}

void CColliderPoint::Update(float fTime)
{
	m_bCollision = false;
}

void CColliderPoint::LateUpdate(float fTime)
{
	m_tInfo = m_tRelative + m_pObj->GetPos();
}

void CColliderPoint::PrevRender(HDC hDC, float fTime)
{
}

void CColliderPoint::Render(HDC hDC, float fTime)
{
#ifdef _DEBUG
	HBRUSH	hBrush = GREENBRUSH;

	if (m_bCollision)
		hBrush = REDBRUSH;

	RECT	tRC = { m_tInfo.x - 5.f, m_tInfo.y - 5.f,
		m_tInfo.x + 5.f, m_tInfo.y + 5.f };

	// UI는 월드 상의 캐릭터들과는 다르게 카메라의 영역에 속해야 한다. 
	// ★ 충돌체도 카메라의 위치설정에 영향을 받는다. 
	if (!m_bIsUI)
	{
		tRC.left -= GET_SINGLE(CCamera)->GetPos().x;
		tRC.top -= GET_SINGLE(CCamera)->GetPos().y;
		tRC.right -= GET_SINGLE(CCamera)->GetPos().x;
		tRC.bottom -= GET_SINGLE(CCamera)->GetPos().y;
	}

	FrameRect(hDC, &tRC, hBrush);
#endif // _DEBUG

}

void CColliderPoint::PostRender(HDC hDC, float fTime)
{
}

CColliderPoint* CColliderPoint::Clone() const
{
	return new CColliderPoint(*this);
}

bool CColliderPoint::Collision(CCollider* pCollider)
{
	switch (pCollider->GetColliderType())
	{
	case CT_RECT:
		return CollisionRectToPoint(pCollider, this);
	case CT_SPHERE:
		return CollisionSphereToPoint(this, pCollider);
	case CT_POINT:
		return CollisionPointToPoint(this, pCollider);
	}

	return false;
}

void CColliderPoint::Save(FILE* pFile)
{
	CCollider::Save(pFile);

	fwrite(&m_tRelative, sizeof(Position), 1, pFile);
}

void CColliderPoint::Load(FILE* pFile)
{
	CCollider::Load(pFile);

	fread(&m_tRelative, sizeof(Position), 1, pFile);
}

void CColliderPoint::UpdateObjPos(const Position& tPos)
{
	/*
		오브젝트 위치정보를 갱신할 때
		충돌체들의 위치에까지 영향을 주게 만들어준 함수
	*/
	m_tInfo = m_tRelative + tPos;
}
