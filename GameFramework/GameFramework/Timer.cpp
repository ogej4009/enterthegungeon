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
	// 윈도우의 고해상도 타이머가 1초당 얼마나 흐르는지를 얻어온다.
	QueryPerformanceFrequency(&m_tSecond);

	// 현재 틱을 미리 얻어둔다.
	QueryPerformanceCounter(&m_tTick);

	m_fDeltaTime = 0.f;
	m_fFPS = 0.f;
	m_iCount = 0;

	return false;
}

float CTimer::Update()
{
	/*
		★ 고 해상도 타이머가 제공하는 값 :		LARGE_INTEGER	m_tSecond
												LARGE_INTEGER	m_tTick

		★ 한 프레임이 돌아가는데 걸린 시간 :	m_fDeltaTime

		★ 1초에 찍는 프레임 횟수 :				Frames Per Second : FPS

		★ 1프레임 찍는 시간 :					m_fFPSTime

		★ 프레임 갯수 :						m_iCount
	*/
	LARGE_INTEGER	tTick;

	// 현재 틱을 얻어온다.
	QueryPerformanceCounter(&tTick);

	// 연산 : 현재 틱 - 과거 틱 / 고해상도 초정보 
	// 결과 : 한 프레임이 돌아가는데 걸린 시간 : m_fDeltaTime
	m_fDeltaTime = (tTick.QuadPart - m_tTick.QuadPart) /
		(float)m_tSecond.QuadPart;

	// 현재 틱을 지정한다. 
	m_tTick = tTick;

	++m_iCount;
	// 1프레임 찍는 시간 + 1프레임이 돌아가는데 걸린 시간을 더한다. 
	// 프레임이 찍히는 시간 따라 시간이 누적된다. 
	// 즉, 전체 프레임이 돌아가는데 걸린 시간 
	m_fFPSTime += m_fDeltaTime;

	/*
			60프레임지정
			60번이 돌았다면
			프레임 갯수 / 전체 프레임이 돌아가는데 걸린 시간 : 결과 : Frames Per Second : FPS	(1초에 찍는 프레임 횟수)
			타이틀바에 찍는 결과 :  m_fFPS, m_fDeltaTime -> 계속 변함
			원하는 결과가 나왔으면
			m_fFPSTime 0으로 초기화
			m_iCount 0으로 초기화

			결론 :
			지속적으로 변하는 값인
			두개의 정보를 확인하기 위함
			1프레임이 돌아가는데 걸린시간 : m_fDeltaTime
			1초에 찍히는 프레임 횟수 : m_fFPS (Frames Per Second : FPS)
		*/
	if (m_iCount == 60)
	{
		m_fFPS = m_iCount / m_fFPSTime;
		m_fFPSTime = 0.f;
		m_iCount = 0;

		// 디버그 모드 일 때 : 타이틀 바에 FPS 상태정보를 보인다
//#ifdef _DEBUG
		char	strFPS[64] = {};

		sprintf_s(strFPS, "FPS : %f, Time : %f", m_fFPS, m_fDeltaTime);

		SetWindowTextA(WINDOWHANDLE, strFPS);
		//#endif // _DEBUG

	}
	// 종료하고,  m_fDeltaTime (1프레임이 돌아가는데 걸린시간)을 리턴한다. 
	return m_fDeltaTime;
}
