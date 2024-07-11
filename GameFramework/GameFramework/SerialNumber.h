#pragma once
#include "Game.h"

class CSerialNumber
{
public:
	CSerialNumber();
	~CSerialNumber();

private:
	unsigned int	m_iNumber; // �ĺ���ȣ
	unsigned int* m_pValidNumber; // ��ȿ�� �ĺ���ȣ 
	unsigned int	m_iSize;
	unsigned int	m_iCapacity;
	unsigned int	m_iMin; /*
									�� ��� �Ϲ� �浹ü�� �ø���ѹ��� 2������ �������� �����.
									����� �Ѿ�� ���콺 �浹ü�� �������� �ʴ´� .
									���콺 �浹ü�� �ø���ѹ��� 2������ ���۵ǵ��� �Ѵ�.
									Collision���� Ư���ϰ� ó���Ѵ�.
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

