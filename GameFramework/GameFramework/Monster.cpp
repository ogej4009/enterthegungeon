#include "Monster.h"
#include "GameManager.h"
#include "Bullet.h"
#include "ColliderRect.h"
#include "ColliderSphere.h"
#include "Math.h"
#include "Animation.h"


// ★ 몬스터의 클립용도 
enum MONSTER_CLIP
{
	MON_IDLE,
	MON_ATTACK,
	MON_DEATH,
	MON_END
};

// ★ 캐릭터의 동작을 식별하기 위한 배열 
const char* g_pMonsterAnimName[SD_END][MON_END] =
{
	{"LeftIdle", "LeftAttack", "LeftDeath"},
	{"RightIdle", "RightAttack", "RightDeath"}
};

CMonster::CMonster() :
	m_pTarget(nullptr)
{
}

CMonster::CMonster(const CMonster& obj) :
	CCharacter(obj)
{
	m_fFireTimeLimit = obj.m_fFireTimeLimit;
	m_fFireTime = 0.f;
	m_eDir = FRONT;
	m_pTarget = nullptr;
}

CMonster::~CMonster()
{
	SAFE_RELEASE(m_pTarget);
}

void CMonster::Start()
{
	CCharacter::Start();
}

bool CMonster::Init()
{
	if (!CCharacter::Init())
		return false;

	// ★ 몬스터 화면설정 
	SetTexture("Monster", TEXT("Teemo_L.bmp"));
	SetSize(200.f, 200.f);
	m_tPos = Position(_RESOLUTION.iWidth - 200.f, 0.f);
	m_eDir = FRONT;
	SetMoveSpeed(200.f);

	// ★ 몬스터 발사체 속도, 시간 지정 
	m_fFireTimeLimit = 1.f;
	m_fFireTime = 0.f;
	/*
		CColliderRect*	pBody = AddCollider<CColliderRect>("MonsterBody");

		pBody->SetRelativeInfo(0.f, 0.f, 200.f, 200.f);

		pBody->SetProfile("Monster");
		pBody->SetChannel("Character");

		SAFE_RELEASE(pBody);
	*/

	// ★ 몬스터 충돌체 설정 
	CColliderSphere* pBody = AddCollider<CColliderSphere>("MonsterBody");
	pBody->SetRelativeInfo(100.f, 100.f, 100.f);
	pBody->SetProfile("Monster");
	pBody->SetChannel("Character");
	SAFE_RELEASE(pBody);

	return true;
}

void CMonster::Update(float fTime)
{
	CCharacter::Update(fTime);

	// ★ 몬스터의 움직임을 결정한다. 
	Move(Position(0.f, 1.f), fTime * m_eDir);

	// 타겟을 따라 움진다.. 
	if (m_pTarget)
	{
		m_fAngle = CMath::GetAngle(m_pTarget->GetPos(), m_tPos);
	}

	MoveAngle(fTime);

	if (m_tPos.y + m_tSize.y >= (float)_RESOLUTION.iHeight)
	{
		m_tPos.y = _RESOLUTION.iHeight - m_tSize.y;
		m_eDir = BACK;
	}

	else if (m_tPos.y <= 0.f)
	{
		m_tPos.y = 0.f;
		m_eDir = FRONT;
	}

	// ★ 몬스터 발사체 위치와 기본설정 
	m_fFireTime += fTime;

	if (m_fFireTime >= m_fFireTimeLimit)
	{
		m_fFireTime -= m_fFireTimeLimit;

		CObj* pBullet = CObj::CreateObject<CBullet>("MonsterBullet", m_strLayerName, m_pScene);

		pBullet->SetTexture("Bullet_L", TEXT("Bullet_L.bmp"));

		((CBullet*)pBullet)->SetMoveDir(BACK);

		pBullet->SetPos(m_tPos.x - pBullet->GetSize().x - 2,
			m_tPos.y + m_tSize.y / 2.f - pBullet->GetSize().y / 2.f);

		// 총알의 위치에서 플레이어를 향한 각도를 구한다.
		float	fAngle = CMath::GetAngle(m_pTarget->GetPos(), pBullet->GetPos());

		((CBullet*)pBullet)->SetAngle(fAngle);

		CCollider* pColl = pBullet->FindCollider("BulletBody");

		if (pColl)
		{
			pColl->SetChannel("MonsterBullet");

			SAFE_RELEASE(pColl);
		}
	}
}

void CMonster::LateUpdate(float fTime)
{
	CCharacter::LateUpdate(fTime);
}

void CMonster::Collision(float fTime)
{
	CCharacter::Collision(fTime);
}

void CMonster::PrevRender(HDC hDC, float fTime)
{
	CCharacter::PrevRender(hDC, fTime);
}

void CMonster::Render(HDC hDC, float fTime)
{
	CCharacter::Render(hDC, fTime);
}

void CMonster::PostRender(HDC hDC, float fTime)
{
	CCharacter::PostRender(hDC, fTime);
}

CMonster* CMonster::Clone()	const
{
	/*
		virtual CObj* Clone()	const;
		virtual CMonster* Clone()	const

	Scene.cpp >>> 여기서 적용

		CObj * CScene::ClonePrototype(const string & strProtoName, const string& strName)
		{
			CObj*	pPrototype = FindPrototype(strProtoName);

			if (!pPrototype)
				return nullptr;

			CObj* pObj = pPrototype->Clone();

			pObj->SetTag(strName);

			return pObj;
		}

	*/
	return new CMonster(*this);
}

void CMonster::CreateClip()
{
	CreateAnimation("MonsterAnimation");

	// ======= Idle
	// Idle 가로 : 82, 세로 : 73
	m_pAnimation->AddClip(g_pMonsterAnimName[SD_LEFT][MON_IDLE], AO_LOOP, 1.f);
	m_pAnimation->SetClipTexture(g_pMonsterAnimName[SD_LEFT][MON_IDLE], "MonsterLeftIdle",
		TEXT("MONSTER/GOWK/L_GOWK_IDLE.bmp"));

	vector<ClipCoord>	vecCoord;
	for (int i = 0; i < 6; ++i)
	{
		ClipCoord	tCoord;
		tCoord.tStart.x = i * 81;
		tCoord.tStart.y = 0;

		tCoord.tEnd.x = (i + 1) * 81;
		tCoord.tEnd.y = 70;

		vecCoord.push_back(tCoord);
	}

	m_pAnimation->AddClipCoord(g_pMonsterAnimName[SD_LEFT][MON_IDLE], vecCoord);

	m_pAnimation->SetTextureColorKey(g_pMonsterAnimName[SD_LEFT][MON_IDLE], 255, 0, 255);

	// ======= Attack
	m_pAnimation->AddClip(g_pMonsterAnimName[SD_LEFT][MON_ATTACK], AO_ONCE_RETURN, 0.3f);
	m_pAnimation->SetClipTexture(g_pMonsterAnimName[SD_LEFT][MON_ATTACK], "MonsterLeftAttack",
		TEXT("MONSTER/GOWK/L_GOWK_IDLE.bmp"));

	vecCoord.clear();
	for (int i = 0; i < 6; ++i)
	{
		ClipCoord	tCoord;
		tCoord.tStart.x = i * 81;
		tCoord.tStart.y = 0;

		tCoord.tEnd.x = (i + 1) * 81;
		tCoord.tEnd.y = 70;

		vecCoord.push_back(tCoord);
	}

	m_pAnimation->AddClipCoord(g_pMonsterAnimName[SD_LEFT][MON_ATTACK], vecCoord);

	m_pAnimation->SetTextureColorKey(g_pMonsterAnimName[SD_LEFT][MON_ATTACK], 255, 0, 255);

	// ======= Death
	m_pAnimation->AddClip(g_pMonsterAnimName[SD_LEFT][MON_DEATH], AO_ONCE_RETURN, 0.3f);
	m_pAnimation->SetClipTexture(g_pMonsterAnimName[SD_LEFT][MON_DEATH], "MonsterLeftDeath",
		TEXT("MONSTER/GOWK/L_GOWK_DEATH.bmp"));

	vecCoord.clear();
	for (int i = 0; i < 4; ++i)
	{
		ClipCoord	tCoord;
		tCoord.tStart.x = i * 81;
		tCoord.tStart.y = 0;

		tCoord.tEnd.x = (i + 1) * 81;
		tCoord.tEnd.y = 70;

		vecCoord.push_back(tCoord);
	}

	m_pAnimation->AddClipCoord(g_pMonsterAnimName[SD_LEFT][MON_DEATH], vecCoord);

	m_pAnimation->SetTextureColorKey(g_pMonsterAnimName[SD_LEFT][MON_DEATH], 255, 0, 255);

	// ======= Idle
	// Idle 가로 : 82, 세로 : 73
	m_pAnimation->AddClip(g_pMonsterAnimName[SD_RIGHT][MON_IDLE], AO_LOOP, 1.f);
	m_pAnimation->SetClipTexture(g_pMonsterAnimName[SD_RIGHT][MON_IDLE], "MonsterRightIdle",
		TEXT("MONSTER/GOWK/R_GOWK_IDLE.bmp"));

	vecCoord.clear();
	for (int i = 0; i < 6; ++i)
	{
		ClipCoord	tCoord;
		tCoord.tStart.x = i * 81;
		tCoord.tStart.y = 0;

		tCoord.tEnd.x = (i + 1) * 81;
		tCoord.tEnd.y = 70;

		vecCoord.push_back(tCoord);
	}

	m_pAnimation->AddClipCoord(g_pMonsterAnimName[SD_RIGHT][MON_IDLE], vecCoord);

	m_pAnimation->SetTextureColorKey(g_pMonsterAnimName[SD_RIGHT][MON_IDLE], 255, 0, 255);

	// ======= Attack
	m_pAnimation->AddClip(g_pMonsterAnimName[SD_RIGHT][MON_ATTACK], AO_ONCE_RETURN, 0.3f);
	m_pAnimation->SetClipTexture(g_pMonsterAnimName[SD_RIGHT][MON_ATTACK], "MonsterRightAttack",
		TEXT("MONSTER/GOWK/R_GOWK_IDLE.bmp"));

	vecCoord.clear();
	for (int i = 0; i < 6; ++i)
	{
		ClipCoord	tCoord;
		tCoord.tStart.x = i * 81;
		tCoord.tStart.y = 0;

		tCoord.tEnd.x = (i + 1) * 81;
		tCoord.tEnd.y = 70;

		vecCoord.push_back(tCoord);
	}

	m_pAnimation->AddClipCoord(g_pMonsterAnimName[SD_RIGHT][MON_ATTACK], vecCoord);

	m_pAnimation->SetTextureColorKey(g_pMonsterAnimName[SD_RIGHT][MON_ATTACK], 255, 0, 255);

	// ======= Death
	m_pAnimation->AddClip(g_pMonsterAnimName[SD_RIGHT][MON_DEATH], AO_ONCE_RETURN, 0.3f);
	m_pAnimation->SetClipTexture(g_pMonsterAnimName[SD_RIGHT][MON_DEATH], "MonsterRightDeath",
		TEXT("MONSTER/GOWK/R_GOWK_DEATH.bmp"));

	vecCoord.clear();
	for (int i = 0; i < 4; ++i)
	{
		ClipCoord	tCoord;
		tCoord.tStart.x = i * 81;
		tCoord.tStart.y = 0;

		tCoord.tEnd.x = (i + 1) * 81;
		tCoord.tEnd.y = 70;

		vecCoord.push_back(tCoord);
	}

	m_pAnimation->AddClipCoord(g_pMonsterAnimName[SD_RIGHT][MON_DEATH], vecCoord);

	m_pAnimation->SetTextureColorKey(g_pMonsterAnimName[SD_RIGHT][MON_DEATH], 255, 0, 255);


}
