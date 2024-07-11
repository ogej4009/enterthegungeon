#include "MainScene.h"
#include "Player.h"
#include "Monster.h"
#include "Bullet.h"
#include "Effect.h"
#include "UIBar.h"
#include "UIImage.h"
#include "Stage.h"
#include "Animation.h"
#include "GameManager.h"
#include "Camera.h"
#include "Solid.h"


CMainScene::CMainScene()
{
}

CMainScene::~CMainScene()
{
	GET_SINGLE(CCamera)->SetTarget(nullptr);
}

void CMainScene::Start()
{
	CScene::Start();
}

bool CMainScene::Init()
{
	CScene::Init();

	// ★ 씬의 크기를 지정한다. 
	m_tSceneSize.x = 8730.f;
	m_tSceneSize.y = 1200.f;

	// ★ 플레이어를 생성한다. 
	CPlayer* pPlayer = CObj::CreateObject<CPlayer>("Player", "Default", this);

	// ★ 몬스터를 생성한다. 
	//CObj*	pMonster = CObj::CreateObject<CMonster>("Monster", "Default", this);
	//((CMonster*)pMonster)->SetTarget(pPlayer);

	// ★ HP
	CObj* pHPBarBorder = CObj::CreateObject<CUIImage>("HPBarBorder", "UI", this);

	pHPBarBorder->SetTexture("HPBarBorder", TEXT("HPBarBack.bmp"));
	pHPBarBorder->SetPos(30.f, 50.f);
	pHPBarBorder->SetSize(120.f, 14.f);
	pHPBarBorder->SetColorKey(255, 0, 255);

	CUIBar* pHPBar = CObj::CreateObject<CUIBar>("HPBar", "UI", this);

	pHPBar->SetMax(1000.f);
	pHPBar->SetValue(1000.f);
	pHPBar->SetPos(40.f, 52.f);

	pPlayer->AddHPCallback<CUIBar>(pHPBar, &CUIBar::SetValue);


	CStage* pStage = CObj::CreateObject<CStage>("MainStage", "BackGround", this);

	//pStage->SetTexture("Stage1", TEXT("Stage1.bmp"));
	//pStage->CreateTile(20, 20, Size(75.f, 60.f));
	pStage->LoadFromFileName("Stage.stg");

	CreatePrototype();

	GET_SINGLE(CCamera)->SetTarget(pPlayer);
	GET_SINGLE(CCamera)->SetPivot(0.5f, 0.5f);

	return true;
}

void CMainScene::CreatePrototype()
{
	// ★ 발사체 프로토타입으로 생성 
	CObj* pBullet = CObj::CreatePrototype<CBullet>("Bullet", this);
	SAFE_RELEASE(pBullet);

	// ★ 타격효과 프로토타입으로 생성 
	CObj* pHitEffect = CObj::CreatePrototype<CEffect>("HitEffect", this);

	pHitEffect->SetPivot(0.5f, 0.5f);
	pHitEffect->CreateAnimation("HitEffect");
	CAnimation* pAnimation = pHitEffect->GetAnimation();

	// Effect : 178, 164
	pAnimation->AddClip("HitEffect", AO_ONCE_DESTROY, 0.5f);
	pAnimation->SetClipTexture("HitEffect", "HitEffect",
		TEXT("Hit.bmp"));

	vector<ClipCoord>	vecCoord;
	for (int i = 0; i < 6; ++i)
	{
		ClipCoord	tCoord;
		tCoord.tStart.x = i * 178;
		tCoord.tStart.y = 0;

		tCoord.tEnd.x = (i + 1) * 178;
		tCoord.tEnd.y = 164;

		vecCoord.push_back(tCoord);
	}

	pAnimation->AddClipCoord("HitEffect", vecCoord);
	pAnimation->SetTextureColorKey("HitEffect", 255, 0, 255);

	SAFE_RELEASE(pHitEffect);

	// ★ 버프효과 프로토타입으로 생성 
	CObj* pBufEffect = CObj::CreatePrototype<CEffect>("BufEffect", this);
	pBufEffect->SetPivot(0.5f, 0.5f);
	pBufEffect->CreateAnimation("BufEffect");
	pAnimation = pBufEffect->GetAnimation();

	// Effect : 178, 164
	pAnimation->AddClip("BufEffect", AO_TIME_DESTROY, 0.5f, 5.f);

	vector<const TCHAR*>	vecFileName;
	vecFileName.push_back(TEXT("Buf1.bmp"));
	vecFileName.push_back(TEXT("Buf2.bmp"));
	vecFileName.push_back(TEXT("Buf3.bmp"));
	pAnimation->SetClipTexture("BufEffect", "BufEffect", vecFileName);

	ClipCoord	tBufCoord;
	tBufCoord.tStart = Position(0.f, 0.f);
	tBufCoord.tEnd = Position(25.f, 25.f);

	pAnimation->AddClipCoord("BufEffect", tBufCoord, 3);

	pAnimation->SetTextureColorKey("BufEffect", 255, 0, 255);

	SAFE_RELEASE(pBufEffect);
}
