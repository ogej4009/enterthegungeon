#include "Solid.h"
#include "ColliderRect.h"


CSolid::CSolid()
{
}

CSolid::CSolid(const CSolid& obj) :
	CMisc(obj)
{
}

CSolid::~CSolid()
{
}

void CSolid::Start()
{
	CMisc::Start();
}

bool CSolid::Init()
{

	SetPos(200.f, 200.f);
	SetPivot(0.f, 0.f);
	SetSize(200.f, 200.f);
	SetTexture("Bonus", TEXT("Bullet_R.bmp"));
	SetColorKey(255, 0, 255);


	CColliderRect* pSolidBody = AddCollider<CColliderRect>("PlayerBody");
	pSolidBody->SetRelativeInfo(-41.f, -73.f, 41.f, 0.f);
	SAFE_RELEASE(pSolidBody);

	return true;
}

void CSolid::Update(float fTime)
{
	CMisc::Update(fTime);
}

void CSolid::LateUpdate(float fTime)
{
	CMisc::LateUpdate(fTime);
}

void CSolid::Collision(float fTime)
{
	CMisc::Collision(fTime);
}

void CSolid::PrevRender(HDC hDC, float fTime)
{
	CMisc::PrevRender(hDC, fTime);
}

void CSolid::Render(HDC hDC, float fTime)
{
	CMisc::Render(hDC, fTime);
}

void CSolid::PostRender(HDC hDC, float fTime)
{
	CMisc::PostRender(hDC, fTime);
}

CMisc* CSolid::Clone() const
{
	return nullptr;
}
