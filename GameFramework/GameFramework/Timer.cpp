#include "Timer.h"
#include "GameManager.h"

CTimer::CTimer()
{
}

CTimer::~CTimer()
{
}

bool CTimer::Init()
{
	// �������� ���ػ� Ÿ�̸Ӱ� 1�ʴ� �󸶳� �帣������ ���´�.
	QueryPerformanceFrequency(&m_tSecond);

	// ���� ƽ�� �̸� ���д�.
	QueryPerformanceCounter(&m_tTick);

	m_fDeltaTime = 0.f;
	m_fFPS = 0.f;
	m_iCount = 0;

	return false;
}

float CTimer::Update()
{
	/*
		�� �� �ػ� Ÿ�̸Ӱ� �����ϴ� �� :		LARGE_INTEGER	m_tSecond
												LARGE_INTEGER	m_tTick

		�� �� �������� ���ư��µ� �ɸ� �ð� :	m_fDeltaTime

		�� 1�ʿ� ��� ������ Ƚ�� :				Frames Per Second : FPS

		�� 1������ ��� �ð� :					m_fFPSTime

		�� ������ ���� :						m_iCount
	*/
	LARGE_INTEGER	tTick;

	// ���� ƽ�� ���´�.
	QueryPerformanceCounter(&tTick);

	// ���� : ���� ƽ - ���� ƽ / ���ػ� ������ 
	// ��� : �� �������� ���ư��µ� �ɸ� �ð� : m_fDeltaTime
	m_fDeltaTime = (tTick.QuadPart - m_tTick.QuadPart) /
		(float)m_tSecond.QuadPart;

	// ���� ƽ�� �����Ѵ�. 
	m_tTick = tTick;

	++m_iCount;
	// 1������ ��� �ð� + 1�������� ���ư��µ� �ɸ� �ð��� ���Ѵ�. 
	// �������� ������ �ð� ���� �ð��� �����ȴ�. 
	// ��, ��ü �������� ���ư��µ� �ɸ� �ð� 
	m_fFPSTime += m_fDeltaTime;

	/*
			60����������
			60���� ���Ҵٸ�
			������ ���� / ��ü �������� ���ư��µ� �ɸ� �ð� : ��� : Frames Per Second : FPS	(1�ʿ� ��� ������ Ƚ��)
			Ÿ��Ʋ�ٿ� ��� ��� :  m_fFPS, m_fDeltaTime -> ��� ����
			���ϴ� ����� ��������
			m_fFPSTime 0���� �ʱ�ȭ
			m_iCount 0���� �ʱ�ȭ

			��� :
			���������� ���ϴ� ����
			�ΰ��� ������ Ȯ���ϱ� ����
			1�������� ���ư��µ� �ɸ��ð� : m_fDeltaTime
			1�ʿ� ������ ������ Ƚ�� : m_fFPS (Frames Per Second : FPS)
		*/
	if (m_iCount == 60)
	{
		m_fFPS = m_iCount / m_fFPSTime;
		m_fFPSTime = 0.f;
		m_iCount = 0;

		// ����� ��� �� �� : Ÿ��Ʋ �ٿ� FPS ���������� ���δ�
//#ifdef _DEBUG
		char	strFPS[64] = {};

		sprintf_s(strFPS, "FPS : %f, Time : %f", m_fFPS, m_fDeltaTime);

		SetWindowTextA(WINDOWHANDLE, strFPS);
		//#endif // _DEBUG

	}
	// �����ϰ�,  m_fDeltaTime (1�������� ���ư��µ� �ɸ��ð�)�� �����Ѵ�. 
	return m_fDeltaTime;
}
