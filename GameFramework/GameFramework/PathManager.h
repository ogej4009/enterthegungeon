#pragma once
#include "Game.h"

class CPathManager
{
private:
	unordered_map<string, TCHAR*>	m_mapPath;
	char	m_strMultibyte[MAX_PATH];

public:
	bool Init();
	void AddPath(const string& strName,
		const TCHAR* pPath,
		const string& strBasePath = ROOT_PATH);
	const TCHAR* FindPath(const string& strName);
	const char* FindPathMultibyte(const string& strName);

	DECLARE_SINGLE(CPathManager)
};