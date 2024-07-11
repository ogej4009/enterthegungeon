#include "Bullet.h"
#include "GameManager.h"
#include "ColliderRect.h"
#include "ColliderSphere.h"
#include "Effect.h"

CBullet::CBullet() :
	m_pTarget(nullptr)
{
}

CBullet::CBullet(const CBullet& obj) :
	CObj(obj)
{
	m_eDir = obj.m_eDir;
	m_pTarget = nullptr;
}

CBullet::~CBullet()
{
	SAFE_RELEASE(m_pTarget);
}

void CBullet::Start()
{
	CObj::Start();

	CCollider* pBody = FindCollider("BulletBody");
	pBody->SetCollisionCallback<CBullet>(CS_BLOCK, this, &CBullet::Hit);
	/*
		SetCollisionCallback<���ü�� �� Ŭ����>(�浹����, ��������Ʈ, ���ε� �Լ��ּ�)
		m_Callback[eState]
		���ε��Լ� Hit : �浹ü���� ȿ���� �߻���Ų��.
	*/
	SAFE_RELEASE(pBody);
}

bool CBullet::Init()
{
	if (!CObj::Init())
		return false;

	// �� �߻�ü�� ȭ�鿡 ��Ÿ����. (�̹�������)
	SetTexture("Bullet_R", TEXT("Bullet_R.bmp"));
	SetSize(100.f, 50.f);
	m_eDir = FRONT;
	SetMoveSpeed(400.f);

	/*
		CColliderRect*	pBody = AddCollider<CColliderRect>("BulletBody");
		pBody->SetRelativeInfo(0.f, 0.f, 100.f, 50.f);
		pBody->SetProfile("Bullet");
		SAFE_RELEASE(pBody);
	*/
	CColliderSphere* pBody = AddCollider<CColliderSphere>("BulletBody");
	pBody->SetRelativeInfo(50.f, 25.f, 50.f); // �浹ü�� ũ�⸦ �����Ѵ�. 
	pBody->SetProfile("Bullet"); // �������� ����
	SAFE_RELEASE(pBody);

	return true;
}

void CBullet::Update(float fTime)
{
	CObj::Update(fTime);


	// Ÿ���� �����Ǹ�, Ÿ���� ��ǥ�� �� ������Ʈ�� ��ǥ�� ����Ͽ� �ڵ����� �ޱ��� �����. 
	if (m_pTarget)
	{
		m_fAngle = CMath::GetAngle(m_pTarget->GetPos(), m_tPos);
	}

	// �̵� �ޱ��� �����Ѵ�. 
	// Move(Position(1.f, 0.f), fTime * m_eDir);
	MoveAngle(fTime);

	// ��ǥ�� �ػ� ������ ������
	if (m_tPos.x >= (float)_RESOLUTION.iWidth)
	{
		Active(false); //�״´�. 
	}

	// ��ǥ�� 0���� ������ 
	else if (m_tPos.x + m_tSize.x <= 0.f)
	{
		Active(false); // �״´�. 
	}
}

void CBullet::LateUpdate(float fTime)
{
	CObj::LateUpdate(fTime);
}

void CBullet::Collision(float fTime)
{
	CObj::Collision(fTime);
}

void CBullet::PrevRender(HDC hDC, float fTime)
{
	CObj::PrevRender(hDC, fTime);
}

void CBullet::Render(HDC hDC, float fTime)
{
	CObj::Render(hDC, fTime);
}

void CBullet::PostRender(HDC hDC, float fTime)
{
	CObj::PostRender(hDC, fTime);
}

CBullet* CBullet::Clone()	const
{
	return new CBullet(*this);
}

void CBullet::Hit(CCollider* pSrc, CCollider* pDest, float fTime)
{
	CObj* pHitEffect = CObj::CreateCloneObject<CEffect>("HitEffect", "HitEffect", m_strLayerName, m_pScene);

	pHitEffect->SetPos(m_tPos);

	Active(false);
}
