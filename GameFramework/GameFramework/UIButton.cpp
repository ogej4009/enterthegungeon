#include "UIButton.h"
#include "ColliderRect.h"
#include "Animation.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "Input.h"

CUIButton::CUIButton()
{
	memset(m_pStateTexture, 0, sizeof(CTexture*) * BS_END);
	m_bMouseOn = false;
	m_eState = BS_NORMAL;
}

CUIButton::CUIButton(const CUIButton& obj) :
	CUI(obj)
{
	m_bMouseOn = false;
}

CUIButton::~CUIButton()
{
	for (int i = 0; i < BS_END; ++i)
	{
		SAFE_RELEASE(m_pStateTexture[i]);
	}
}

void CUIButton::Start()
{
	CUI::Start();
}

bool CUIButton::Init()
{
	if (!CUI::Init())
		return false;

	SetSize(100.f, 50.f);
	SetPivot(0.5f, 0.5f);


	// ★  마우스와 UI버튼의 4가지 상태들 

	GET_SINGLE(CResourceManager)->LoadTexture("ButtonDefaultNormal", TEXT("ButtonDefault_Normal.bmp"));
	m_pStateTexture[BS_NORMAL] = GET_SINGLE(CResourceManager)->FindTexture("ButtonDefaultNormal");

	GET_SINGLE(CResourceManager)->LoadTexture("ButtonDefaultMouseOn", TEXT("ButtonDefault_MouseOn.bmp"));
	m_pStateTexture[BS_MOUSEON] = GET_SINGLE(CResourceManager)->FindTexture("ButtonDefaultMouseOn");

	GET_SINGLE(CResourceManager)->LoadTexture("ButtonDefaultClick", TEXT("ButtonDefault_Click.bmp"));
	m_pStateTexture[BS_CLICK] = GET_SINGLE(CResourceManager)->FindTexture("ButtonDefaultClick");

	GET_SINGLE(CResourceManager)->LoadTexture("ButtonDefaultDisEnable", TEXT("ButtonDefault_DisEnable.bmp"));
	m_pStateTexture[BS_DISENABLE] = GET_SINGLE(CResourceManager)->FindTexture("ButtonDefaultDisEnable");

	SetTexture(m_pStateTexture[BS_NORMAL]);

	m_eState = BS_NORMAL;
	m_ePrevState = BS_NORMAL;

	CColliderRect* pBody = AddCollider<CColliderRect>("ButtonBody");

	pBody->SetRelativeInfo(-50.f, -25.f, 50.f, 25.f);

	pBody->SetProfile("Button");
	pBody->SetChannel("Button");
	pBody->EnableUI();
	pBody->SetCollisionCallback<CUIButton>(CS_BLOCK,
		this, &CUIButton::Hit);
	pBody->SetCollisionCallback<CUIButton>(CS_RELEASE,
		this, &CUIButton::HitRelease);

	SAFE_RELEASE(pBody);

	return true;
}

void CUIButton::Update(float fTime)
{
	CUI::Update(fTime);
}

void CUIButton::LateUpdate(float fTime)
{
	CUI::LateUpdate(fTime);

	if (m_bMouseOn)
	{
		if (GET_SINGLE(CInput)->GetLButtonInfo().bDown)
		{
			m_eState = BS_CLICK;
		}

		else if (GET_SINGLE(CInput)->GetLButtonInfo().bUp &&
			m_eState == BS_CLICK)
		{
			m_eState = BS_MOUSEON;

			if (m_StateCallback[BS_CLICK] != nullptr)
				m_StateCallback[BS_CLICK](fTime);
		}
	}
	/*
		텍스쳐 교체 : 현재 상태의 텍스쳐로 교체한다.
	*/
	if (m_ePrevState != m_eState)
	{
		m_ePrevState = m_eState;

		SetTexture(m_pStateTexture[m_eState]);
	}
}

void CUIButton::Collision(float fTime)
{
	CUI::Collision(fTime);
}

void CUIButton::PrevRender(HDC hDC, float fTime)
{
	CUI::PrevRender(hDC, fTime);
}

void CUIButton::Render(HDC hDC, float fTime)
{
	CUI::Render(hDC, fTime);
}

void CUIButton::PostRender(HDC hDC, float fTime)
{
	CUI::PostRender(hDC, fTime);
}

CUIButton* CUIButton::Clone() const
{
	return new CUIButton(*this);
}

void CUIButton::Hit(CCollider* pSrc, CCollider* pDest,
	float fTime)
{
	/*
		input.cpp 에서 생성한 마우스 충돌체 : MouseUI
	*/
	if (m_eState == BS_DISENABLE)
		return;

	if (pDest->GetTag() == "MouseUI")
	{
		m_eState = BS_MOUSEON;
		m_bMouseOn = true; // 상태에 따라서 텍스쳐를 다르게 쓴다.
	}
}

void CUIButton::HitRelease(CCollider* pSrc, CCollider* pDest, float fTime)
{
	if (m_eState == BS_DISENABLE)
		return;

	if (pDest->GetTag() == "MouseUI")
	{
		m_bMouseOn = false;
		m_eState = BS_NORMAL;
	}
}