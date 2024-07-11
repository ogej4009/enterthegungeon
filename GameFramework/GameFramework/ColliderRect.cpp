#include "ColliderRect.h"
#include "Obj.h"
#include "GameManager.h"
#include "Camera.h"

CColliderRect::CColliderRect()
{
	// 생성자에서 자신의 충돌 타입이 CT_RECT로 지정되게 한다. 
	m_eColliderType = CT_RECT;
	SetTag("Rect");
}

CColliderRect::CColliderRect(const CColliderRect& collider) :
	CCollider(collider)
{
	m_tRelative = collider.m_tRelative;
	m_tInfo = collider.m_tInfo;
}

CColliderRect::~CColliderRect()
{
}

void CColliderRect::Start()
{
	CCollider::Start();
}

bool CColliderRect::Init()
{
	return true;
}

void CColliderRect::Update(float fTime)
{
	m_bCollision = false;
}

void CColliderRect::LateUpdate(float fTime)
{
	m_tInfo.l = m_tRelative.l + m_pObj->GetPos().x;
	m_tInfo.t = m_tRelative.t + m_pObj->GetPos().y;
	m_tInfo.r = m_tRelative.r + m_pObj->GetPos().x;
	m_tInfo.b = m_tRelative.b + m_pObj->GetPos().y;
}

void CColliderRect::PrevRender(HDC hDC, float fTime)
{
}

void CColliderRect::Render(HDC hDC, float fTime)
{
#ifdef _DEBUG
	HBRUSH	hBrush = GREENBRUSH;

	if (m_bCollision)
		hBrush = REDBRUSH;

	RECT	tRC = { m_tInfo.l, m_tInfo.t, m_tInfo.r, m_tInfo.b };

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

	TCHAR	strNumber[32] = {};
	// 문자열을 만들어주는 함수 : 유니코드 : int 를 문자로 바꿔줄 수 있는 함수
	wsprintf(strNumber, TEXT("SerialNumber : %d"), m_iSerialNumber);
	// 위에서 넘어온 strNumber을 화면에 그려준다. 
	TextOut(hDC, tRC.left, tRC.top, strNumber, lstrlen(strNumber));

#endif // _DEBUG

}

void CColliderRect::PostRender(HDC hDC, float fTime)
{
}

CColliderRect* CColliderRect::Clone() const
{
	return new CColliderRect(*this);
}

bool CColliderRect::Collision(CCollider* pCollider)
{// 들어온 상대 충돌체의 타입을 분별한다. 
	switch (pCollider->GetColliderType())
	{
		// 다양한 모양이 존재할 때, 그들끼리 충돌을 판단해 주는 함수
	case CT_RECT:
		return CollisionRectToRect(this, pCollider);
	case CT_SPHERE:
		return CollisionRectToSphere(this, pCollider);
	case CT_POINT:
		return CollisionRectToPoint(this, pCollider);
	}

	return false;
}

void CColliderRect::Save(FILE* pFile)
{
	CCollider::Save(pFile);

	fwrite(&m_tRelative, sizeof(RectInfo), 1, pFile);
}

void CColliderRect::Load(FILE* pFile)
{
	CCollider::Load(pFile);

	fread(&m_tRelative, sizeof(RectInfo), 1, pFile);
}

void CColliderRect::UpdateObjPos(const Position& tPos)
{
	/*
		오브젝트 위치정보를 갱신할 때
		충돌체들의 위치에까지 영향을 주게 만들어준 함수
	*/
	m_tInfo.l = m_tRelative.l + tPos.x;
	m_tInfo.t = m_tRelative.t + tPos.y;
	m_tInfo.r = m_tRelative.r + tPos.x;
	m_tInfo.b = m_tRelative.b + tPos.y;
}
