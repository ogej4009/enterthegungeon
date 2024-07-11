#include "SerialNumber.h"

CSerialNumber::CSerialNumber()
{
	m_iMin = 0;
	m_iSize = 0;
	m_iNumber = m_iMin; /*
									＠ 모든 일반 충돌체는 시리얼넘버를 2번부터 가져가게 만든다.
									장면이 넘어가도 마우스 충돌체는 지워지지 않는다 .
									마우스 충돌체는 시리얼넘버가 2번부터 시작되도록 한다.
									Collision에서 특별하게 처리한다.
						*/
	m_iCapacity = 16;
	m_pValidNumber = new unsigned int[m_iCapacity];
}

CSerialNumber::~CSerialNumber()
{
	SAFE_DELETE_ARRAY(m_pValidNumber);
}

// 식별 번호 부여 
unsigned int CSerialNumber::CreateSerialNumber()
{
	if (m_iSize > 0)
	{
		--m_iSize;
		return m_pValidNumber[m_iSize];
	}

	int	iNumber = m_iNumber;
	++m_iNumber;

	return iNumber;
}

// 쓰고 재활용하기 위해 반납되는 식별번호 함수 
void CSerialNumber::ReturnSerialNumber(unsigned int iNumber)
{
	if (m_iSize == m_iCapacity)
	{
		m_iCapacity *= 2;

		unsigned int* pArray = new unsigned int[m_iCapacity];
		memset(pArray, 0, sizeof(unsigned int) * m_iCapacity);

		memcpy(pArray, m_pValidNumber, sizeof(unsigned int) * m_iSize);

		SAFE_DELETE_ARRAY(m_pValidNumber);

		m_pValidNumber = pArray;
	}

	m_pValidNumber[m_iSize] = iNumber;
	++m_iSize;
}
