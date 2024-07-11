#pragma once
#include "Game.h"


class CTimer
{
	friend class CGameManager;

private:
	CTimer();
	~CTimer();

private:
	LARGE_INTEGER	m_tSecond;
	LARGE_INTEGER	m_tTick;
	float			m_fDeltaTime;
	float			m_fFPS;
	float			m_fFPSTime;
	int				m_iCount;

public:
	float GetDeltaTime()	const
	{
		return m_fDeltaTime;
	}

public:
	bool Init();
	float Update();
};

