#include "Player.h"
#include "Bullet.h"
#include "ColliderRect.h"
#include "ColliderSphere.h"
#include "Animation.h"
#include "Effect.h"

// ★ 플레이어의 클립용도 
enum PLAYER_CLIP
{
	PL_IDLE,
	PL_ATTACK,
	PL_WALK,
	PL_END
};

// ★ 캐릭터의 동작을 식별하기 위한 배열 
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

	// 이동 키에서 호출해줄 함수를 등록한다.
	m_pInputComponent->BindAxis<CPlayer>("MoveFront", this, &CPlayer::MoveFront, "MoveFront");
	m_pInputComponent->BindAxis<CPlayer>("MoveSide", this, &CPlayer::MoveSide, "MoveSide");

	m_pInputComponent->BindAction<CPlayer>("Dash", IE_DOUBLE, this, &CPlayer::Dash, "Dash");
	m_pInputComponent->BindAction<CPlayer>("Attack", IE_HOLD, this, &CPlayer::Fire, "Fire");
	m_pInputComponent->BindAction<CPlayer>("Skill1", IE_PRESS, this, &CPlayer::Skill1, "Skill1");
	m_pInputComponent->BindAction<CPlayer>("Skill2", IE_PRESS, this, &CPlayer::Skill2, "Skill2");

	// ★ 플레이어 좌표 이미지 설정 
	SetTexture("Player", TEXT("PlayerWoman/Right/astand.bmp"));
	SetColorKey(255, 0, 255);
	SetSize(82.f, 73.f);
	SetPivot(0.5f, 1.f);
	SetTileColSize(82.f, 73.f); // ☆ 유일한 타일 사이즈 등록 
	SetPos(200.f, 50.f);
	SetMoveSpeed(500.f);

	// ★ 플레이어 충돌체 등록 
	CColliderRect* pBody = AddCollider<CColliderRect>("PlayerBody");
	pBody->SetRelativeInfo(-41.f, -73.f, 41.f, 0.f);
	pBody->SetProfile("Player");
	pBody->SetChannel("Character");
	pBody->SetCollisionCallback<CPlayer>(CS_BLOCK, this, &CPlayer::Hit);
	SAFE_RELEASE(pBody);

	// 플레이어의 중력을 인식 
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

// ★ 방향전환 
void CPlayer::MoveFront(float fScale, float fTime)
{
	Move(Position(0.f, 1.f), fScale * fTime);

	m_pAnimation->ChangeClip(g_pAnimName[m_eSideDir][PL_WALK]);
}

// ★ 방향전환 
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

// ★ 대쉬(평소보다 느림)
void CPlayer::Dash(float fTime)
{
	m_tPos.x += 100.f;
}

// ★ 발사 상황 
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

// ★ ctrl + 1
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

// ★ ctrl + 2
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

// ★ HP콜백함수 
void CPlayer::Hit(CCollider* pSrc, CCollider* pDest, float fTime)
{
	m_iHP -= 50;

	for (size_t i = 0; i < m_vecHPCallback.size(); ++i)
	{
		m_vecHPCallback[i](m_iHP);
	}
}

// ★ 플레이어가 클립으로 저장할 6가지 상태 
void CPlayer::CreateClip()
{
	CreateAnimation("PlayerAnimation");

	// ======= Idle
	// Idle 가로 : 82, 세로 : 73
	m_pAnimation->AddClip(g_pAnimName[SD_LEFT][PL_IDLE], AO_ONCE_FRAMESTOP, 1.f);
	m_pAnimation->SetClipTexture(g_pAnimName[SD_LEFT][PL_IDLE], "PlayerLeftIdle",
		TEXT("PlayerWoman/Left/astand_left.bmp"));
	m_pAnimation->SetStopFrame(g_pAnimName[SD_LEFT][PL_IDLE], 4); //★ 사용법 

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
	// Idle 가로 : 82, 세로 : 73
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
