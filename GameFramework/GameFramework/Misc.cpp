#include "Misc.h"


CMisc::CMisc()
{

}

CMisc::CMisc(const CMisc& obj) :
	CObj(obj)
{

}

CMisc::~CMisc()
{

}

void CMisc::Start()
{
	CObj::Start();
}

bool CMisc::Init()
{
	return true;
}

void CMisc::Update(float fTime)
{
	CObj::Update(fTime);
}

void CMisc::LateUpdate(float fTime)
{
	CObj::LateUpdate(fTime);
}

void CMisc::Collision(float fTime)
{
	CObj::Collision(fTime);
}

void CMisc::PrevRender(HDC hDC, float fTime)
{
	CObj::PrevRender(hDC, fTime);
}

void CMisc::Render(HDC hDC, float fTime)
{
	CObj::Render(hDC, fTime);
}

void CMisc::PostRender(HDC hDC, float fTime)
{
	CObj::PostRender(hDC, fTime);
}

CMisc* CMisc::Clone() const
{
	return nullptr;
}

