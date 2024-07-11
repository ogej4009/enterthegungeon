#include "Character.h"
#include "Input.h"
#include "InputComponent.h"


CCharacter::CCharacter() :
	m_pInputComponent(nullptr)
{
}

CCharacter::CCharacter(const CCharacter& obj) :
	CObj(obj)
{
}

CCharacter::~CCharacter()
{
	SAFE_RELEASE(m_pInputComponent);
}

void CCharacter::Start()
{
	CObj::Start();
}

bool CCharacter::Init()
{
	m_pInputComponent = new CInputComponent;

	return true;
}

void CCharacter::Update(float fTime)
{
	CObj::Update(fTime);
}

void CCharacter::LateUpdate(float fTime)
{
	CObj::LateUpdate(fTime);
}

void CCharacter::Collision(float fTime)
{
	CObj::Collision(fTime);
}

void CCharacter::PrevRender(HDC hDC, float fTime)
{
	CObj::PrevRender(hDC, fTime);
}

void CCharacter::Render(HDC hDC, float fTime)
{
	CObj::Render(hDC, fTime);
}

void CCharacter::PostRender(HDC hDC, float fTime)
{
	CObj::PostRender(hDC, fTime);
}

CCharacter* CCharacter::Clone()	const
{
	return nullptr;
}
