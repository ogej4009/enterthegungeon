#include "StartScene.h"
#include "UIButton.h"
#include "GameManager.h"
#include "SceneManager.h"
#include "MainScene.h"
#include "EditScene.h"
#include "Camera.h"
#include "SoundManager.h"


CStartScene::CStartScene()
{
}

CStartScene::~CStartScene()
{
}

void CStartScene::Start()
{
	CScene::Start();
}

bool CStartScene::Init()
{
	CScene::Init();

	GET_SINGLE(CSoundManager)->LoadSound("BGM", true, "MainBgm.mp3");

	GET_SINGLE(CSoundManager)->Play("BGM", CT_BGM);


	GET_SINGLE(CSoundManager)->LoadSound("ButtonEffect", false, "1Up.wav");

	// ★ 씬의 사이즈를 지정한다. 
	m_tSceneSize.x = _RESOLUTION.iWidth;
	m_tSceneSize.y = _RESOLUTION.iHeight;

	// ☆ 배경의 이미지를 세팅한다. 
	/*CObj*	pStartImage = CObj::CreateObject<CStartImage>("StartImage", "BackGround", this);
	pStartImage->SetTexture("StartImage", TEXT("StartImage.bmp"));
	pStartImage->SetPos(0, 0);
	pStartImage->SetSize(1500, 1200);
	pStartImage->SetColorKey(255, 0, 255);
*/
// ★ 버튼을 지정한다. 
	CObj* pStartButton = CObj::CreateObject<CUIButton>("StartButton", "UI", this);
	pStartButton->SetPos(_RESOLUTION.iWidth / 2.f, _RESOLUTION.iHeight / 2.f);
	((CUIButton*)pStartButton)->SetButtonStateCallback<CStartScene>(BS_CLICK, this, &CStartScene::StartButton);

	// ★ 버튼을 지정한다. 
	CObj* pEditButton = CObj::CreateObject<CUIButton>("EditButton", "UI", this);
	pEditButton->SetPos(_RESOLUTION.iWidth / 2.f, _RESOLUTION.iHeight / 2.f + 100.f);
	((CUIButton*)pEditButton)->SetButtonStateCallback<CStartScene>(BS_CLICK, this, &CStartScene::EditButton);

	//((CUIButton*)pStartButton)->DisEnable(true);

	return true;
}

void CStartScene::StartButton(float fTime)
{
	GET_SINGLE(CSoundManager)->Play("ButtonEffect", CT_EFFECT);
	// ★ 장면전환한다. 
	//MessageBox(nullptr, TEXT("시작버튼"), TEXT("시작버튼"), MB_OK);
	GET_SINGLE(CSceneManager)->CreateScene<CMainScene>("MainScene");
}

void CStartScene::EditButton(float fTime)
{
	// ★ 장면전환한다. 
	GET_SINGLE(CSceneManager)->CreateScene<CEditScene>("EditScene");
}

void CStartScene::ExitButton(float fTime)
{
}
