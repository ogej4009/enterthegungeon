#include "UI.h"


CUI::CUI()
{
	m_bCameraEnable = false;
	m_bIsUI = true;
}

CUI::CUI(const CUI& obj) :
	CObj(obj)
{
	m_bCameraEnable = false;
	m_bIsUI = true;
}

CUI::~CUI()
{
}

void CUI::Start()
{
	CObj::Start();
}

bool CUI::Init()
{
	if (!CObj::Init())
		return false;

	m_strLayerName = "UI";

	return true;
}

void CUI::Update(float fTime)
{
	CObj::Update(fTime);
}

void CUI::LateUpdate(float fTime)
{
	CObj::LateUpdate(fTime);
}

void CUI::Collision(float fTime)
{
	CObj::Collision(fTime);
}

void CUI::PrevRender(HDC hDC, float fTime)
{
	CObj::PrevRender(hDC, fTime);
}

void CUI::Render(HDC hDC, float fTime)
{
	CObj::Render(hDC, fTime);
}

void CUI::PostRender(HDC hDC, float fTime)
{
	CObj::PostRender(hDC, fTime);
}

CUI* CUI::Clone() const
{
	return nullptr;
}
