#include "Effect.h"


CEffect::CEffect()
{
}

CEffect::CEffect(const CEffect& obj) :
	CObj(obj)
{
}

CEffect::~CEffect()
{
}

void CEffect::Start()
{
	CObj::Start();
}

bool CEffect::Init()
{
	if (!CObj::Init())
		return false;

	return true;
}

void CEffect::Update(float fTime)
{
	CObj::Update(fTime);
}

void CEffect::LateUpdate(float fTime)
{
	CObj::LateUpdate(fTime);
}

void CEffect::Collision(float fTime)
{
	CObj::Collision(fTime);
}

void CEffect::PrevRender(HDC hDC, float fTime)
{
	CObj::PrevRender(hDC, fTime);
}

void CEffect::Render(HDC hDC, float fTime)
{
	CObj::Render(hDC, fTime);
}

void CEffect::PostRender(HDC hDC, float fTime)
{
	CObj::PostRender(hDC, fTime);
}

CEffect* CEffect::Clone() const
{
	return new CEffect(*this);
}
