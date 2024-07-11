#pragma once
#include "Game.h"

class CSerialNumber
{
public:
	CSerialNumber();
	~CSerialNumber();

private:
	unsigned int	m_iNumber; // 식별번호
	unsigned int* m_pValidNumber; // 유효한 식별번호 
	unsigned int	m_iSize;
	unsigned int	m_iCapacity;
	unsigned int	m_iMin; /*
									＠ 모든 일반 충돌체는 시리얼넘버를 2번부터 가져가게 만든다.
									장면이 넘어가도 마우스 충돌체는 지워지지 않는다 .
									마우스 충돌체는 시리얼넘버가 2번부터 시작되도록 한다.
									Collision에서 특별하게 처리한다.
							*/

public:
	void SetMin(unsigned int iMin)
	{
		m_iMin = iMin;
		m_iNumber = m_iMin;
	}

	unsigned int CreateSerialNumber();
	void ReturnSerialNumber(unsigned int iNumber);
};

