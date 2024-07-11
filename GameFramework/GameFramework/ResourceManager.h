#pragma once
#include "Game.h"


class CResourceManager
{
private:
	HINSTANCE	m_hInst;
	HDC			m_hDC;
	HWND		m_hWnd;

private:
	unordered_map<string, class CTexture*>	m_mapTexture;

public:
	bool Init();
	bool LoadTexture(const string& strName,
		const TCHAR* pFileName,
		const string& strPathName = TEXTURE_PATH);
	bool LoadTexture(const string& strName,
		const vector<const TCHAR*>& vecFileName,
		const string& strPathName = TEXTURE_PATH);
	void ClearScene();

public:
	class CTexture* FindTexture(const string& strName);

	DECLARE_SINGLE(CResourceManager)
};

