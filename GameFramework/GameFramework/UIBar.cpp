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

	// �� UI-HPBAR�� �ؽ��� ���� 
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

	//�� UI-HPBAR�� �������� ������. 
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
	// ��ȭ�ϴ� ���� �����. 
	float	fPercent = (m_fValue - m_fMin) / (m_fMax - m_fMin);

	// ��ǥ�� ���Ѵ�. 
	Position	tWorldPos = m_tPos;

	// bar�� �Ǻ��� �������� �ʰ� ��� ������ ���Ѵ�. 
	// �ٸ� ������ �´� ���̸� ���Ѵ�. 
	Size	tSize = m_tSize;

	// ���� ���� ��� x���̸� ���ҽ�Ų��.
	tSize.x *= fPercent;

	// �� ������Ÿ���� Ŭ������ : �ؽ��ĸ� Ȯ���ϰ�, Ŭ���� �׷����� ��� 
	if (m_pTexture)
	{
		Position	tImagePos;

		// ó�� ��ȣ�� 0�̴�. 
		int	iFrame = 0;

		// �ִϸ��̼� ��ü�� �ٸ� ���� �ּҰ� �Ҵ�� ��� 
		if (m_pAnimation)
		{
			PAnimationClip	pClip = m_pAnimation->GetClip();// Ŭ�� ������ �����´�.  

			// �ش� Ŭ���� �ִ� ���� ��ǥ�� ��������, �Ҵ��Ѵ�.
			tImagePos = pClip->vecCoord[pClip->iFrame].tStart;

			// Ÿ���� �������� ���, �� Ŭ���� �������� ����  iFrame���� ��ü�Ѵ�. 
			if (pClip->eType == AT_FRAME)
				iFrame = pClip->iFrame;
		}
		// �� �ؽ��ĸ� �׷�����. 
		m_pTexture->Render(hDC, tWorldPos, tImagePos, tSize, iFrame);
	}

	// �浹ü ����Ʈ���� ���� �Ͱ� ��Ȱ��ȭ ��ü���� �ɷ�����, �ʿ��� �͸� ȭ�鿡 �׸���. 
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
