#include "Ref.h"

CRef::CRef()
{
	// 객체가 생성되면 무조건 +1 
	m_iRef = 1;
	// 객체가 생성되면 활성화
	m_bEnable = true;
	// 객체가 생성되면 ALIVE
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
