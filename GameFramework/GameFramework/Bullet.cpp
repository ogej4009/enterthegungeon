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
		SetCollisionCallback<대상객체의 본 클래스>(충돌상태, 대상오브젝트, 바인딩 함수주소)
		m_Callback[eState]
		바인딩함수 Hit : 충돌체간의 효과를 발생시킨다.
	*/
	SAFE_RELEASE(pBody);
}

bool CBullet::Init()
{
	if (!CObj::Init())
		return false;

	// ★ 발사체를 화면에 나타낸다. (이미지파일)
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
	pBody->SetRelativeInfo(50.f, 25.f, 50.f); // 충돌체의 크기를 결정한다. 
	pBody->SetProfile("Bullet"); // 프로파일 지정
	SAFE_RELEASE(pBody);

	return true;
}

void CBullet::Update(float fTime)
{
	CObj::Update(fTime);


	// 타겟이 지정되면, 타겟의 좌표와 현 오브젝트의 좌표를 계산하여 자동으로 앵글을 만든다. 
	if (m_pTarget)
	{
		m_fAngle = CMath::GetAngle(m_pTarget->GetPos(), m_tPos);
	}

	// 이동 앵글을 설정한다. 
	// Move(Position(1.f, 0.f), fTime * m_eDir);
	MoveAngle(fTime);

	// 좌표가 해상도 밖으로 나가면
	if (m_tPos.x >= (float)_RESOLUTION.iWidth)
	{
		Active(false); //죽는다. 
	}

	// 좌표가 0보다 작으면 
	else if (m_tPos.x + m_tSize.x <= 0.f)
	{
		Active(false); // 죽는다. 
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
