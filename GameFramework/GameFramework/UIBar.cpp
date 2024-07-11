#include "UIBar.h"
#include "ColliderRect.h"
#include "Animation.h"
#include "Texture.h"
#include "ResourceManager.h"

CUIBar::CUIBar()
{
	m_fMin = 0.f;
	m_fMax = 1.f;
	m_fValue = 1.f;
}

CUIBar::CUIBar(const CUIBar& obj) :
	CUI(obj)
{
	m_fMin = obj.m_fMin;
	m_fMax = obj.m_fMax;
	m_fValue = obj.m_fValue;
}

CUIBar::~CUIBar()
{
}

void CUIBar::Start()
{
	CUI::Start();
}

bool CUIBar::Init()
{
	if (!CUI::Init())
		return false;

	// ★ UI-HPBAR의 텍스쳐 지정 
	SetSize(100.f, 10.f);

	SetTexture("BarDefault", TEXT("HPBar.bmp"));

	return true;
}

void CUIBar::Update(float fTime)
{
	CUI::Update(fTime);
}

void CUIBar::LateUpdate(float fTime)
{
	CUI::LateUpdate(fTime);

	//★ UI-HPBAR의 데미지를 입힌다. 
	if (m_fValue < m_fMin)
		m_fValue = m_fMin;

	else if (m_fValue > m_fMax)
		m_fValue = m_fMax;
}

void CUIBar::Collision(float fTime)
{
	CUI::Collision(fTime);
}

void CUIBar::PrevRender(HDC hDC, float fTime)
{
	CUI::PrevRender(hDC, fTime);
}

void CUIBar::Render(HDC hDC, float fTime)
{
	// 변화하는 값을 만든다. 
	float	fPercent = (m_fValue - m_fMin) / (m_fMax - m_fMin);

	// 좌표를 구한다. 
	Position	tWorldPos = m_tPos;

	// bar는 피봇을 적용하지 않고 대신 비율을 정한다. 
	// 다만 비율에 맞는 길이를 구한다. 
	Size	tSize = m_tSize;

	// 원본 길이 대비 x길이를 감소시킨다.
	tSize.x *= fPercent;

	// ☆ 프레임타입의 클립정보 : 텍스쳐를 확인하고, 클립을 그려내는 방법 
	if (m_pTexture)
	{
		Position	tImagePos;

		// 처음 번호는 0이다. 
		int	iFrame = 0;

		// 애니메이션 객체에 다른 것의 주소가 할당된 경우 
		if (m_pAnimation)
		{
			PAnimationClip	pClip = m_pAnimation->GetClip();// 클립 정보를 가져온다.  

			// 해당 클립에 있는 시작 좌표를 가져오고, 할당한다.
			tImagePos = pClip->vecCoord[pClip->iFrame].tStart;

			// 타입이 프레임인 경우, 그 클립의 프레임을 변수  iFrame으로 교체한다. 
			if (pClip->eType == AT_FRAME)
				iFrame = pClip->iFrame;
		}
		// ★ 텍스쳐를 그려낸다. 
		m_pTexture->Render(hDC, tWorldPos, tImagePos, tSize, iFrame);
	}

	// 충돌체 리스트에서 죽은 것과 비활성화 객체들을 걸러내고, 필요한 것만 화면에 그린다. 
	list<CCollider*>::iterator	iter;
	list<CCollider*>::iterator	iterEnd = m_ColliderList.end();

	for (iter = m_ColliderList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			SAFE_RELEASE((*iter));
			iter = m_ColliderList.erase(iter);
			iterEnd = m_ColliderList.end();
			continue;
		}

		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		(*iter)->Render(hDC, fTime);
		++iter;
	}
}

void CUIBar::PostRender(HDC hDC, float fTime)
{
	CUI::PostRender(hDC, fTime);
}

CUIBar* CUIBar::Clone() const
{
	return new CUIBar(*this);
}
