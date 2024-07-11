#include "Player.h"
#include "Bullet.h"
#include "ColliderRect.h"
#include "ColliderSphere.h"
#include "Animation.h"
#include "Effect.h"

// �� �÷��̾��� Ŭ���뵵 
enum PLAYER_CLIP
{
	PL_IDLE,
	PL_ATTACK,
	PL_WALK,
	PL_END
};

// �� ĳ������ ������ �ĺ��ϱ� ���� �迭 
const char* g_pAnimName[SD_END][PL_END] =
{
	{"LeftIdle", "LeftAttack", "LeftWalk"},
	{"RightIdle", "RightAttack", "RightWalk"}
};

CPlayer::CPlayer()
{
	m_iHPMax = 1000.f;
	m_iHP = 1000.f;
}

CPlayer::CPlayer(const CPlayer& obj) :
	CCharacter(obj)
{
	m_eSideDir = obj.m_eSideDir;
}

CPlayer::~CPlayer()
{
}

void CPlayer::Start()
{
	CCharacter::Start();
}

bool CPlayer::Init()
{
	if (!CCharacter::Init())
		return false;

	m_eSideDir = SD_RIGHT;

	// �̵� Ű���� ȣ������ �Լ��� ����Ѵ�.
	m_pInputComponent->BindAxis<CPlayer>("MoveFront", this, &CPlayer::MoveFront, "MoveFront");
	m_pInputComponent->BindAxis<CPlayer>("MoveSide", this, &CPlayer::MoveSide, "MoveSide");

	m_pInputComponent->BindAction<CPlayer>("Dash", IE_DOUBLE, this, &CPlayer::Dash, "Dash");
	m_pInputComponent->BindAction<CPlayer>("Attack", IE_HOLD, this, &CPlayer::Fire, "Fire");
	m_pInputComponent->BindAction<CPlayer>("Skill1", IE_PRESS, this, &CPlayer::Skill1, "Skill1");
	m_pInputComponent->BindAction<CPlayer>("Skill2", IE_PRESS, this, &CPlayer::Skill2, "Skill2");

	// �� �÷��̾� ��ǥ �̹��� ���� 
	SetTexture("Player", TEXT("PlayerWoman/Right/astand.bmp"));
	SetColorKey(255, 0, 255);
	SetSize(82.f, 73.f);
	SetPivot(0.5f, 1.f);
	SetTileColSize(82.f, 73.f); // �� ������ Ÿ�� ������ ��� 
	SetPos(200.f, 50.f);
	SetMoveSpeed(500.f);

	// �� �÷��̾� �浹ü ��� 
	CColliderRect* pBody = AddCollider<CColliderRect>("PlayerBody");
	pBody->SetRelativeInfo(-41.f, -73.f, 41.f, 0.f);
	pBody->SetProfile("Player");
	pBody->SetChannel("Character");
	pBody->SetCollisionCallback<CPlayer>(CS_BLOCK, this, &CPlayer::Hit);
	SAFE_RELEASE(pBody);

	// �÷��̾��� �߷��� �ν� 
	UseGravity(true);

	/*
		CColliderSphere*	pBody = AddCollider<CColliderSphere>("PlayerBody");

		pBody->SetRelativeInfo(-0.f, 0.f, 100.f);

		pBody->SetProfile("Player");
		pBody->SetChannel("Character");
		pBody->SetCollisionCallback<CPlayer>(CS_BLOCK,
			this, &CPlayer::Hit);

		SAFE_RELEASE(pBody);
	*/
	CreateClip();

	m_pAnimation->SetDefaultClip(g_pAnimName[m_eSideDir][PL_IDLE]);

	return true;
}

void CPlayer::Update(float fTime)
{
	CCharacter::Update(fTime);
}

void CPlayer::LateUpdate(float fTime)
{
	CCharacter::LateUpdate(fTime);
}

void CPlayer::Collision(float fTime)
{
	CCharacter::Collision(fTime);
}

void CPlayer::PrevRender(HDC hDC, float fTime)
{
	CCharacter::PrevRender(hDC, fTime);
}

void CPlayer::Render(HDC hDC, float fTime)
{
	CCharacter::Render(hDC, fTime);
}

void CPlayer::PostRender(HDC hDC, float fTime)
{
	CCharacter::PostRender(hDC, fTime);
}

CPlayer* CPlayer::Clone()	const
{
	return new CPlayer(*this);
}

// �� ������ȯ 
void CPlayer::MoveFront(float fScale, float fTime)
{
	Move(Position(0.f, 1.f), fScale * fTime);

	m_pAnimation->ChangeClip(g_pAnimName[m_eSideDir][PL_WALK]);
}

// �� ������ȯ 
void CPlayer::MoveSide(float fScale, float fTime)
{
	Move(Position(1.f, 0.f), fScale * fTime);

	if (fScale == 1.f)
		m_eSideDir = SD_RIGHT;

	else if (fScale == -1)
		m_eSideDir = SD_LEFT;

	m_pAnimation->ChangeClip(g_pAnimName[m_eSideDir][PL_WALK]);

	m_pAnimation->SetDefaultClip(g_pAnimName[m_eSideDir][PL_IDLE]);
}

// �� �뽬(��Һ��� ����)
void CPlayer::Dash(float fTime)
{
	m_tPos.x += 100.f;
}

// �� �߻� ��Ȳ 
void CPlayer::Fire(float fTime)
{
	m_pAnimation->ChangeClip(g_pAnimName[m_eSideDir][PL_ATTACK]);

	CObj* pBullet = CObj::CreateCloneObject<CBullet>("Bullet", "PlayerBullet", m_strLayerName, m_pScene);

	pBullet->SetGravityForce(300.f);
	pBullet->UseGravity(true);
	pBullet->SetPos(m_tPos.x + m_tSize.x, m_tPos.y - pBullet->GetSize().y / 2.f);

	CCollider* pColl = pBullet->FindCollider("BulletBody");

	if (pColl)
	{
		pColl->SetChannel("PlayerBullet");

		SAFE_RELEASE(pColl);
	}
}

// �� ctrl + 1
void CPlayer::Skill1(float fTime)
{
	CObj* pBullet = CObj::CreateCloneObject<CBullet>("Bullet", "PlayerBullet", m_strLayerName, m_pScene);

	pBullet->SetPos(m_tPos.x + m_tSize.x, m_tPos.y - pBullet->GetSize().y / 2.f);

	CObj* pMonster = FindObject("Monster");

	((CBullet*)pBullet)->SetTarget(pMonster);

	SAFE_RELEASE(pMonster);

	CCollider* pColl = pBullet->FindCollider("BulletBody");

	if (pColl)
	{
		pColl->SetChannel("PlayerBullet");

		SAFE_RELEASE(pColl);
	}
}

// �� ctrl + 2
void CPlayer::Skill2(float fTime)
{
	CObj* pBufEffect = FindObject("PlayerBuf1Effect");

	if (pBufEffect)
	{
		pBufEffect->GetAnimation()->ClearOptionTime();

		SAFE_RELEASE(pBufEffect);
	}

	else
	{
		pBufEffect = CObj::CreateCloneObject<CEffect>("BufEffect",
			"PlayerBuf1Effect", m_strLayerName, m_pScene);

		pBufEffect->SetPos(m_tPos + Position(0.f, -30.f));
	}
}

// �� HP�ݹ��Լ� 
void CPlayer::Hit(CCollider* pSrc, CCollider* pDest, float fTime)
{
	m_iHP -= 50;

	for (size_t i = 0; i < m_vecHPCallback.size(); ++i)
	{
		m_vecHPCallback[i](m_iHP);
	}
}

// �� �÷��̾ Ŭ������ ������ 6���� ���� 
void CPlayer::CreateClip()
{
	CreateAnimation("PlayerAnimation");

	// ======= Idle
	// Idle ���� : 82, ���� : 73
	m_pAnimation->AddClip(g_pAnimName[SD_LEFT][PL_IDLE], AO_ONCE_FRAMESTOP, 1.f);
	m_pAnimation->SetClipTexture(g_pAnimName[SD_LEFT][PL_IDLE], "PlayerLeftIdle",
		TEXT("PlayerWoman/Left/astand_left.bmp"));
	m_pAnimation->SetStopFrame(g_pAnimName[SD_LEFT][PL_IDLE], 4); //�� ���� 

	vector<ClipCoord>	vecCoord;
	for (int i = 0; i < 6; ++i)
	{
		ClipCoord	tCoord;
		tCoord.tStart.x = i * 82;
		tCoord.tStart.y = 0;

		tCoord.tEnd.x = (i + 1) * 82;
		tCoord.tEnd.y = 73;

		vecCoord.push_back(tCoord);
	}

	m_pAnimation->AddClipCoord(g_pAnimName[SD_LEFT][PL_IDLE], vecCoord);
	m_pAnimation->SetTextureColorKey(g_pAnimName[SD_LEFT][PL_IDLE], 255, 0, 255);

	// ======= Attack
	m_pAnimation->AddClip(g_pAnimName[SD_LEFT][PL_ATTACK], AO_ONCE_RETURN, 0.3f);
	m_pAnimation->SetClipTexture(g_pAnimName[SD_LEFT][PL_ATTACK], "PlayerLeftAttack",
		TEXT("PlayerWoman/Left/aswing_left.bmp"));

	vecCoord.clear();
	for (int i = 0; i < 3; ++i)
	{
		ClipCoord	tCoord;
		tCoord.tStart.x = i * 176;
		tCoord.tStart.y = 0;

		tCoord.tEnd.x = (i + 1) * 176;
		tCoord.tEnd.y = 89;

		vecCoord.push_back(tCoord);
	}

	m_pAnimation->AddClipCoord(g_pAnimName[SD_LEFT][PL_ATTACK], vecCoord);
	m_pAnimation->SetTextureColorKey(g_pAnimName[SD_LEFT][PL_ATTACK], 255, 0, 255);

	// ======= Walk
	m_pAnimation->AddClip(g_pAnimName[SD_LEFT][PL_WALK], AO_ONCE_RETURN, 0.3f);
	m_pAnimation->SetClipTexture(g_pAnimName[SD_LEFT][PL_WALK], "PlayerLeftWalk",
		TEXT("PlayerWoman/Left/awalk_left.bmp"));

	vecCoord.clear();
	for (int i = 0; i < 4; ++i)
	{
		ClipCoord	tCoord;
		tCoord.tStart.x = i * 85;
		tCoord.tStart.y = 0;

		tCoord.tEnd.x = (i + 1) * 85;
		tCoord.tEnd.y = 75;

		vecCoord.push_back(tCoord);
	}

	m_pAnimation->AddClipCoord(g_pAnimName[SD_LEFT][PL_WALK], vecCoord);
	m_pAnimation->SetTextureColorKey(g_pAnimName[SD_LEFT][PL_WALK], 255, 0, 255);

	// ======= Idle
	// Idle ���� : 82, ���� : 73
	m_pAnimation->AddClip(g_pAnimName[SD_RIGHT][PL_IDLE], AO_LOOP, 1.f);
	m_pAnimation->SetClipTexture(g_pAnimName[SD_RIGHT][PL_IDLE], "PlayerRightIdle",
		TEXT("PlayerWoman/Right/astand.bmp"));

	vecCoord.clear();
	for (int i = 0; i < 6; ++i)
	{
		ClipCoord	tCoord;
		tCoord.tStart.x = i * 82;
		tCoord.tStart.y = 0;

		tCoord.tEnd.x = (i + 1) * 82;
		tCoord.tEnd.y = 73;

		vecCoord.push_back(tCoord);
	}

	m_pAnimation->AddClipCoord(g_pAnimName[SD_RIGHT][PL_IDLE], vecCoord);
	m_pAnimation->SetTextureColorKey(g_pAnimName[SD_RIGHT][PL_IDLE], 255, 0, 255);

	// ======= Attack
	m_pAnimation->AddClip(g_pAnimName[SD_RIGHT][PL_ATTACK], AO_ONCE_RETURN, 0.3f);
	m_pAnimation->SetClipTexture(g_pAnimName[SD_RIGHT][PL_ATTACK], "PlayerRightAttack",
		TEXT("PlayerWoman/Right/aswing.bmp"));

	vecCoord.clear();
	for (int i = 0; i < 3; ++i)
	{
		ClipCoord	tCoord;
		tCoord.tStart.x = i * 176;
		tCoord.tStart.y = 0;

		tCoord.tEnd.x = (i + 1) * 176;
		tCoord.tEnd.y = 89;

		vecCoord.push_back(tCoord);
	}

	m_pAnimation->AddClipCoord(g_pAnimName[SD_RIGHT][PL_ATTACK], vecCoord);
	m_pAnimation->SetTextureColorKey(g_pAnimName[SD_RIGHT][PL_ATTACK], 255, 0, 255);

	// ======= Walk
	m_pAnimation->AddClip(g_pAnimName[SD_RIGHT][PL_WALK], AO_ONCE_RETURN, 0.3f);
	m_pAnimation->SetClipTexture(g_pAnimName[SD_RIGHT][PL_WALK], "PlayerRightWalk",
		TEXT("PlayerWoman/Right/awalk.bmp"));

	vecCoord.clear();
	for (int i = 0; i < 4; ++i)
	{
		ClipCoord	tCoord;
		tCoord.tStart.x = i * 85;
		tCoord.tStart.y = 0;

		tCoord.tEnd.x = (i + 1) * 85;
		tCoord.tEnd.y = 75;

		vecCoord.push_back(tCoord);
	}

	m_pAnimation->AddClipCoord(g_pAnimName[SD_RIGHT][PL_WALK], vecCoord);
	m_pAnimation->SetTextureColorKey(g_pAnimName[SD_RIGHT][PL_WALK], 255, 0, 255);
}
