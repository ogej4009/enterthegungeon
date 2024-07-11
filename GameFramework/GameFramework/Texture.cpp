#include "Texture.h"
#include "PathManager.h"
#include "GameManager.h"


CTexture::CTexture()
{
	m_bColorKey = false;
	m_iColorKey = 0;
}

CTexture::~CTexture()
{
	Safe_Delete_Array_VecList(m_vecFileName);

	// ������ �����Ǿ� �ִ� ������ �����Ѵ�.
	for (size_t i = 0; i < m_vecDC.size(); ++i)
	{
		SelectObject(m_vecDC[i], m_vecPrevBmp[i]);
		DeleteObject(m_vecBmp[i]);
		DeleteDC(m_vecDC[i]);
	}
}

bool CTexture::LoadTexture(const string& strName,
	const TCHAR* pFileName,
	const string& strPathName)
{
	SetTag(strName);

	TCHAR* pSaveName = new TCHAR[MAX_PATH];
	memset(pSaveName, 0, sizeof(TCHAR) * MAX_PATH);

	lstrcpy(pSaveName, pFileName);

	m_vecFileName.push_back(pSaveName);

	m_strPathName = strPathName;

	// ��ü Ǯ��θ� ������ش�.
	TCHAR	strPath[MAX_PATH] = {};

	const TCHAR* pPath = GET_SINGLE(CPathManager)->FindPath(strPathName);

	if (pPath)
		lstrcpy(strPath, pPath);

	lstrcat(strPath, pFileName);

	// ȭ���� DC�� �̿��ؼ� �޸� DC�� ������.
	HDC	hDC = CreateCompatibleDC(WINDOWDC);

	// �̹����� �ε��Ѵ�.
	HBITMAP hBmp = (HBITMAP)LoadImage(WINDOWINSTANCE, strPath, IMAGE_BITMAP,
		0, 0, LR_LOADFROMFILE);

	// �޸� DC�� ��Ʈ�� �׸��� ������ �����Ѵ�.
	// �Ʒ� �Լ��� ������ �����Ǿ� �ִ� �⺻ ������
	// ��ȯ�ȴ�.
	HBITMAP hPrevBmp = (HBITMAP)SelectObject(hDC, hBmp);

	// ��Ʈ�� ������ ���´�.
	BITMAP	tInfo;
	GetObject(hBmp, sizeof(BITMAP), &tInfo);

	m_vecDC.push_back(hDC);
	m_vecBmp.push_back(hBmp);
	m_vecPrevBmp.push_back(hPrevBmp);
	m_vecBmpInfo.push_back(tInfo);

	return true;
}

bool CTexture::LoadTexture(const string& strName,
	const vector<const TCHAR*>& vecFileName, const string& strPathName)
{
	for (size_t i = 0; i < vecFileName.size(); ++i)
	{
		if (!LoadTexture(strName, vecFileName[i], strPathName))
			return false;
	}

	return true;
}

void CTexture::Render(HDC hDC, float fDestX, float fDestY,
	float fSrcX, float fSrcY, float fSizeX, float fSizeY, int iFrame)
{
	// 1�����ڴ� ����� ȭ���� DC�̴�.
	// 2�����ڴ� ����� DC������ X��ǥ�̴�.
	// 3�����ڴ� ����� DC������ Y��ǥ�̴�.
	// 4�����ڴ� ����� ����� ���� ũ���̴�.
	// 5�����ڴ� ����� ����� ���� ũ���̴�.
	// 6�����ڴ� ����� ����� DC�̴�.
	// 7�� ���ڴ� ����� ����� �̹����󿡼��� �׸� ������ X�̴�.
	// 8�� ���ڴ� ����� ����� �̹����󿡼��� �׸� ������ Y�̴�.
	// ������ ���ڴ� ���� ����̴�.
	// SRCCOPY : �� DC�� ��� DC�� �������ش�.
	// BLACKNESS : ��󿵿��� Black���� ĥ�Ѵ�.
	// WHITENESS : ��󿵿��� White���� ĥ�Ѵ�.

	/*
		 �̹����� �� ���� ���� ������ ���
		 TransparentBlt : �̹����� ���������� �ϴ� ���� �ƴ϶� ������������ �������־ ����� ������ �� �ִ�.
		 m_iColorKey�� ȭ�鿡�� ���� ���� �� �ִ�. �Ⱦ��� ������ ������ ���ϵ�
		 �̹����� ���������� ���������� ���ؼ� ������ ������ :
		 �ִϸ��̼� ���� + �÷�Ű���� �����ؼ� ������ ���� : Ȯ�� ���ϱ� ������ ���� ũ��� ����
	*/
	if (!m_bColorKey)
		BitBlt(hDC, fDestX, fDestY, fSizeX, fSizeY, m_vecDC[iFrame], fSrcX, fSrcY, SRCCOPY);

	else
		TransparentBlt(hDC, fDestX, fDestY, fSizeX, fSizeY, m_vecDC[iFrame], fSrcX, fSrcY, fSizeX, fSizeY, m_iColorKey);
}

void CTexture::Render(HDC hDC, const Position& tDestPos,
	const Position& tSrcPos, const Size& tSize, int iFrame)
{

	// �÷� Ű ��� ���θ� �и��ؼ� BitBlt�� TransparentBlt�� ������. 
	if (!m_bColorKey)
		BitBlt(hDC, tDestPos.x, tDestPos.y, tSize.x, tSize.y, m_vecDC[iFrame], tSrcPos.x, tSrcPos.y, SRCCOPY);

	else
		TransparentBlt(hDC, tDestPos.x, tDestPos.y, tSize.x, tSize.y, m_vecDC[iFrame], tSrcPos.x, tSrcPos.y, tSize.x, tSize.y, m_iColorKey);
}

void CTexture::Save(FILE* pFile)
{
	int	iLength = m_strTag.length();
	fwrite(&iLength, sizeof(int), 1, pFile);
	fwrite(m_strTag.c_str(), 1, iLength, pFile);

	// ���� ��θ� �����Ѵ�.
	size_t iCount = m_vecFileName.size();

	fwrite(&iCount, sizeof(size_t), 1, pFile);

	for (size_t i = 0; i < iCount; ++i)
	{
		iLength = lstrlen(m_vecFileName[i]);
		fwrite(&iLength, sizeof(int), 1, pFile);
		fwrite(m_vecFileName[i], sizeof(TCHAR), iLength, pFile);
	}

	iLength = m_strPathName.length();
	fwrite(&iLength, sizeof(int), 1, pFile);
	fwrite(m_strPathName.c_str(), sizeof(char), iLength, pFile);

	fwrite(&m_iColorKey, sizeof(unsigned int), 1, pFile);
	fwrite(&m_bColorKey, sizeof(bool), 1, pFile);
}

void CTexture::Load(FILE* pFile)
{
	fread(&m_iColorKey, sizeof(unsigned int), 1, pFile);
	fread(&m_bColorKey, sizeof(bool), 1, pFile);
}
