#include "StartImage.h"


CStartImage::CStartImage()
{
}

CStartImage::CStartImage(const CStartImage& obj)
{
}


CStartImage::~CStartImage()
{
}

void CStartImage::Start()
{
}

bool CStartImage::Init()
{
	CUI::Init();

	SetSize(1500.f, 1200.f);

	SetTexture("Player_Left", TEXT("Player_Left.bmp"));

	return true;
}

void CStartImage::Update(float fTime)
{
}

void CStartImage::LateUpdate(float fTime)
{
}

void CStartImage::Collision(float fTime)
{
}

void CStartImage::PrevRender(HDC hDC, float fTime)
{
}

void CStartImage::Render(HDC hDC, float fTime)
{
}

void CStartImage::PostRender(HDC hDC, float fTime)
{
}

CStartImage* CStartImage::Clone() const
{
	return nullptr;
}