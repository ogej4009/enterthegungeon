#include "Ref.h"

CRef::CRef()
{
	// ��ü�� �����Ǹ� ������ +1 
	m_iRef = 1;
	// ��ü�� �����Ǹ� Ȱ��ȭ
	m_bEnable = true;
	// ��ü�� �����Ǹ� ALIVE
	m_bActive = true;
}

CRef::~CRef()
{
}

void CRef::AddRef()
{
	++m_iRef;
}

int CRef::Release()
{
	--m_iRef;

	if (m_iRef == 0)
	{
		delete	this;
		return 0;
	}

	return m_iRef;
}
