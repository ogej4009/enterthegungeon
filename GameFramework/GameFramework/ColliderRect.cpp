#include "ColliderRect.h"
#include "Obj.h"
#include "GameManager.h"
#include "Camera.h"

CColliderRect::CColliderRect()
{
	// �����ڿ��� �ڽ��� �浹 Ÿ���� CT_RECT�� �����ǰ� �Ѵ�. 
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

	// UI�� ���� ���� ĳ���͵���� �ٸ��� ī�޶��� ������ ���ؾ� �Ѵ�. 
	// �� �浹ü�� ī�޶��� ��ġ������ ������ �޴´�. 
	if (!m_bIsUI)
	{
		tRC.left -= GET_SINGLE(CCamera)->GetPos().x;
		tRC.top -= GET_SINGLE(CCamera)->GetPos().y;
		tRC.right -= GET_SINGLE(CCamera)->GetPos().x;
		tRC.bottom -= GET_SINGLE(CCamera)->GetPos().y;
	}

	FrameRect(hDC, &tRC, hBrush);

	TCHAR	strNumber[32] = {};
	// ���ڿ��� ������ִ� �Լ� : �����ڵ� : int �� ���ڷ� �ٲ��� �� �ִ� �Լ�
	wsprintf(strNumber, TEXT("SerialNumber : %d"), m_iSerialNumber);
	// ������ �Ѿ�� strNumber�� ȭ�鿡 �׷��ش�. 
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
{// ���� ��� �浹ü�� Ÿ���� �к��Ѵ�. 
	switch (pCollider->GetColliderType())
	{
		// �پ��� ����� ������ ��, �׵鳢�� �浹�� �Ǵ��� �ִ� �Լ�
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
		������Ʈ ��ġ������ ������ ��
		�浹ü���� ��ġ������ ������ �ְ� ������� �Լ�
	*/
	m_tInfo.l = m_tRelative.l + tPos.x;
	m_tInfo.t = m_tRelative.t + tPos.y;
	m_tInfo.r = m_tRelative.r + tPos.x;
	m_tInfo.b = m_tRelative.b + tPos.y;
}
