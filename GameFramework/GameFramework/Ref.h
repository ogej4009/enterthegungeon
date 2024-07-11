#pragma once
#include "Game.h"

class CRef
{
public:
	CRef();
	virtual ~CRef() = 0; // ＠ 완전가상소멸자

protected:
	int		m_iRef;
	string	m_strTag;
	bool	m_bEnable;
	bool	m_bActive;

public:
	void AddRef();
	int Release();

	void SetTag(const string& strTag)
	{
		m_strTag = strTag;
	}

	string GetTag()	const
	{
		return m_strTag;
	}

	void Active(bool bActive)
	{
		m_bActive = bActive; // ＠ ALIVE
	}

	bool IsActive()	const
	{
		return m_bActive; // ＠ ALIVE
	}

	void Enable(bool bEnable)
	{
		m_bEnable = bEnable; // ＠ 활성화 
	}

	bool IsEnable()	const
	{
		return m_bEnable; // ＠ 활성화
	}
};

