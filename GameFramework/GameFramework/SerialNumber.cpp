#include "SerialNumber.h"

CSerialNumber::CSerialNumber()
{
	m_iMin = 0;
	m_iSize = 0;
	m_iNumber = m_iMin; /*
									�� ��� �Ϲ� �浹ü�� �ø���ѹ��� 2������ �������� �����.
									����� �Ѿ�� ���콺 �浹ü�� �������� �ʴ´� .
									���콺 �浹ü�� �ø���ѹ��� 2������ ���۵ǵ��� �Ѵ�.
									Collision���� Ư���ϰ� ó���Ѵ�.
						*/
	m_iCapacity = 16;
	m_pValidNumber = new unsigned int[m_iCapacity];
}

CSerialNumber::~CSerialNumber()
{
	SAFE_DELETE_ARRAY(m_pValidNumber);
}

// �ĺ� ��ȣ �ο� 
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

// ���� ��Ȱ���ϱ� ���� �ݳ��Ǵ� �ĺ���ȣ �Լ� 
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
