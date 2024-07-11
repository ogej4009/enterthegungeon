#pragma once
#include "Ref.h"


class CTexture :
	public CRef
{
	friend class CResourceManager;

private:
	CTexture();
	~CTexture();

private:
	vector<HDC>			m_vecDC;
	vector<HBITMAP>		m_vecBmp;
	vector<HBITMAP>		m_vecPrevBmp;
	vector<BITMAP>		m_vecBmpInfo;
	unsigned int	m_iColorKey;
	bool			m_bColorKey;

	vector<TCHAR*>		m_vecFileName;
	string				m_strPathName;

public:
	HDC GetDC(int iFrame = 0)	const
	{
		return m_vecDC[iFrame];
	}

	HBITMAP GetBmp(int iFrame = 0)	const
	{
		return m_vecBmp[iFrame];
	}

	BITMAP GetBitmapInfo(int iFrame = 0)	const
	{
		return m_vecBmpInfo[iFrame];
	}

	void SetColorKey(unsigned char r, unsigned char g, unsigned char b)
	{
		m_bColorKey = true;
		m_iColorKey = RGB(r, g, b);
	}

public:
	bool LoadTexture(const string& strName,
		const TCHAR* pFileName,
		const string& strPathName = TEXTURE_PATH);
	bool LoadTexture(const string& strName,
		const vector<const TCHAR*>& vecFileName,
		const string& strPathName = TEXTURE_PATH);
	void Render(HDC hDC, float fDestX, float fDestY, float fSrcX, float fSrcY,
		float fSizeX, float fSizeY, int iFrame = 0);
	void Render(HDC hDC, const Position& tDestPos, const Position& tSrcPos,
		const Size& tSize, int iFrame = 0);
	void Save(FILE* pFile);
	void Load(FILE* pFile);
};

