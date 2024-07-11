#include "ColliderSphere.h"
#include "Obj.h"
#include "GameManager.h"
#include "Camera.h"

CColliderSphere::CColliderSphere()
{
	// �����ڿ��� �ڽ��� �浹 Ÿ���� CT_RECT�� �����ǰ� �Ѵ�. 
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
	// ������(r) : ���� ������ ��ġ + ������ 
	m_tInfo.tCenter = m_tRelative.tCenter + m_pObj->GetPos();
}

void CColliderSphere::PrevRender(HDC hDC, float fTime)
{
}

void CColliderSphere::Render(HDC hDC, float fTime)
{
#ifdef _DEBUG
	HPEN	hPen = GREENPEN;

	// �浹�ϸ� '����'�� ���Ѵ�
	if (m_bCollision)
		hPen = REDPEN;

	HPEN	hPrevPen = (HPEN)SelectObject(hDC, hPen);

	Position	tLinePos[17]; // ��ǥ���� 

	// ���Լ��� �̿��� ��ǥ�� ����Ѵ�. 
	for (int i = 0; i < 17; ++i)
	{
		tLinePos[i].x = m_tInfo.tCenter.x + cosf(CMath::AngleToRadian(i * 22.5f)) * m_tInfo.fRadius;
		tLinePos[i].y = m_tInfo.tCenter.y + sinf(CMath::AngleToRadian(i * 22.5f)) * m_tInfo.fRadius;
	}

	Position	tPos = tLinePos[0]; // ������ 

	// UI�� ���� ���� ĳ���͵���� �ٸ��� ī�޶��� ������ ���ؾ� �Ѵ�. 
	if (!m_bIsUI)
		tPos -= GET_SINGLE(CCamera)->GetPos(); // ���� �浹ü�� ī�޶��� ������ �޴´�. 

	MoveToEx(hDC, tPos.x, tPos.y, nullptr);

	// �� ����ϰ� �׷��ش�. 
	for (int i = 1; i < 17; ++i)
	{
		tPos = tLinePos[i];

		// �� �浹ü�� ī�޶��� ������ �޴´�.  
		// UI�� ���� ���� ĳ���͵���� �ٸ��� ī�޶��� ������ ���ؾ� �Ѵ�. 
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
	// �浹ü�� ��翡 ���� �ٸ��� ó�� 
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
		������Ʈ ��ġ������ ������ ��
		�浹ü���� ��ġ������ ������ �ְ� ������� �Լ�
	*/
	// ������(r) : ���� ������ ��ġ + ������ 
	m_tInfo.tCenter = m_tRelative.tCenter + tPos;
}
