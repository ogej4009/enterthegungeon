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
			// ������Ŀ��� ������ �浹�� �ǰ� �ִٸ� ��� �������ش�.
			if (GET_SINGLE(CCollisionManager)->CheckPrevCollisionAdj(m_iSerialNumber,
				(*iter)->GetSerialNumber()))// �ø���ѹ��� �����. 
			{
				GET_SINGLE(CCollisionManager)->DeletePrevCollisionAdj(m_iSerialNumber,
					(*iter)->GetSerialNumber());// �ø���ѹ��� �����. 

				(*iter)->DeletePrevCollider(this);// �浹ü��ü�� �����. 

				Call(CS_RELEASE, *iter, fTime);
				(*iter)->Call(CS_RELEASE, this, fTime);
			}
		}
		// �浹�Ŵ����� �ø���ѹ��� �����Ѵ�.
		GET_SINGLE(CCollisionManager)->ReturnSerialNumber(m_iSerialNumber);
	}
}

void CCollider::SetProfile(const string& strName)
{
	/*
		�������� �� ������ ������ �ٲٸ� �������� ����Ʈ ���� ���� ���� �� ä�� �Ѱ��� ���õǾ����
		�浹 �Ŵ������� ���������� �̸��� ������
		����Ʈ ä�� �ε����� ������ ����
		���� ���� ä���� ���� ������ �ؼ� ���� ä���� ã�Ƽ� �״��� setprofile�Լ� ȣ���ϸ�,
		�� ���������� ����Ʈ ä���ε����� ������ �����Ƿ� ���� ���� ȣ���� SetProfileä���� ������ ���ŵȴ� . �����.
	*/
	m_pProfile = GET_SINGLE(CCollisionManager)->FindProfile(strName);
}

void CCollider::SetChannel(const string& strName)
{
	/*
		ä���� �����Ѵٴ� ���� �����迭�� ����� ä�ε��� �ε�����ȣ�� �����ͼ� � ��ȣ���� ���� �浹ü�� ������ ���ִ� ���̴�.
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
		Obj�� Start���� �浹ü ��ü���� �ϳ��� ȣ���Ѵ�.
		�浹ü �Ŵ����� �ø���ѹ��� �ο��Ѵ�.
		�浹ü �Ŵ����� �� �浹ü��ü�� ����Ѵ�. �� ������ �浹ü���� �ڽ��� �浹ü �Ŵ����� ����Ѵ�.
	*/
	// �浹ü �����ڴ� �ø���ѹ��� �ο��Ѵ�. 
	m_iSerialNumber = GET_SINGLE(CCollisionManager)->CreateSerialNumber();
	// �ڱ��ڽ��� �浹ü�� ����Ѵ�. �浹ü �����ڴ� �� �浹ü�� ����Ʈ�� �����Ѵ�. 
	GET_SINGLE(CCollisionManager)->AddCollider(this);
}

bool CCollider::Init()
{
	// �������ϰ� ä�� �Ѵ� init���� ������ ���� �� �ְ� �����. 
	// �浹�Ŵ����� FindProfile
	m_pProfile = GET_SINGLE(CCollisionManager)->FindProfile("BlockAll");
	// GetChannelIndex�� ä���� �ε����� �ο��Ѵ�.
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
		������Ʈ ��ġ������ ������ ��
		�浹ü���� ��ġ������ ������ �ְ� ������� �Լ�
	*/
}

bool CCollider::CollisionRectToRect(CCollider* pSrc,
	CCollider* pDest)
{
	Position	tIntersect; // ���� 

	if (CollisionRectToRect(((CColliderRect*)pSrc)->GetInfo(),
		((CColliderRect*)pDest)->GetInfo(), tIntersect))
	{
		// ������ �����. 
		pSrc->m_tIntersect = tIntersect;
		pDest->m_tIntersect = tIntersect;

		pSrc->m_bCollision = true;
		pDest->m_bCollision = true;

		// �浹 �Ǿ���. 
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
	// rect �𼭸� l, t, r, b			  ���ǽ�	    	true	false
	// ���׿����� : if�б⹮ �����ϰ�  tSrc.l > tDest.l ? tSrc.l : tDest.l;
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
	// �� ���� �������� �Ÿ��� ���Ѵ�.
	float	fDist = tSrc.tCenter.Distance(tDest.tCenter);

	if (fDist > tSrc.fRadius + tDest.fRadius)
		return false;

	// Src�� �������� Dest�� �������� ���� ��� Src�� �������� ��ġ�� ����
	// �κ��� ���̸� ���Ѵ�.
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

	// ��ġ�� ������ ���̸� ���Ѵ�.
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

	// �������� ������ ���Ѵ�.
	float	fAngle = CMath::GetAngle(tSrc.tCenter, tDest.tCenter);

	tIntersect.x = tSrc.tCenter.x + cosf(CMath::AngleToRadian(fAngle)) * fLength;
	tIntersect.y = tSrc.tCenter.y + sinf(CMath::AngleToRadian(fAngle)) * fLength;

	return true;
}

bool CCollider::CollisionRectToSphere(const RectInfo& tSrc,
	const SphereInfo& tDest, Position& tIntersect)
{
	// ���� ������ �簢���� ��, �� �ȿ� ���Եǰų�
	// �簢���� ��, �Ʒ� �ȿ� ���Ե� ��� �簢���� ���� ��������ŭ
	// Ȯ���ؼ� ������ Ȯ��� �簢���� �������� �Ǵ��Ѵ�.
	if ((tSrc.l <= tDest.tCenter.x && tDest.tCenter.x <= tSrc.r) ||
		(tSrc.t <= tDest.tCenter.y && tDest.tCenter.y <= tSrc.b))
	{
		// ��������ŭ Ȯ��� �簢���� �����.
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
		// rect �𼭸� l, t, r, b			  ���ǽ�	    	true	false
		// ���׿����� : if�б⹮ �����ϰ�  tSrc.l > tDest.l ? tSrc.l : tDest.l;
		l = tSrc.l > tRC.l ? tSrc.l : tRC.l;
		r = tSrc.r < tRC.r ? tSrc.r : tRC.r;
		t = tSrc.t > tRC.t ? tSrc.t : tRC.t;
		b = tSrc.b < tRC.b ? tSrc.b : tRC.b;

		tIntersect.x = (l + r) / 2.f;
		tIntersect.y = (t + b) / 2.f;

		return true;
	}

	// �簢���� �����ϴ� 4���� ���� �� �ȿ� �ϳ��� ���ٸ�
	// �浹�̴�.
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

			// rect �𼭸� l, t, r, b			  ���ǽ�	    	true	false
			// ���׿����� : if�б⹮ �����ϰ�  tSrc.l > tDest.l ? tSrc.l : tDest.l;
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
