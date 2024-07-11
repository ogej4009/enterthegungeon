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

	// 기존에 지정되어 있던 도구를 지정한다.
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

	// 전체 풀경로를 만들어준다.
	TCHAR	strPath[MAX_PATH] = {};

	const TCHAR* pPath = GET_SINGLE(CPathManager)->FindPath(strPathName);

	if (pPath)
		lstrcpy(strPath, pPath);

	lstrcat(strPath, pFileName);

	// 화면의 DC를 이용해서 메모리 DC를 만들어낸다.
	HDC	hDC = CreateCompatibleDC(WINDOWDC);

	// 이미지를 로딩한다.
	HBITMAP hBmp = (HBITMAP)LoadImage(WINDOWINSTANCE, strPath, IMAGE_BITMAP,
		0, 0, LR_LOADFROMFILE);

	// 메모리 DC에 비트맵 그리기 도구를 지정한다.
	// 아래 함수는 기존에 지정되어 있던 기본 도구가
	// 반환된다.
	HBITMAP hPrevBmp = (HBITMAP)SelectObject(hDC, hBmp);

	// 비트맵 정보를 얻어온다.
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
	// 1번인자는 출력할 화면의 DC이다.
	// 2번인자는 출력할 DC에서의 X좌표이다.
	// 3번인자는 출력할 DC에서의 Y좌표이다.
	// 4번인자는 출력할 대상의 가로 크기이다.
	// 5번인자는 출력할 대상의 세로 크기이다.
	// 6번인자는 출력할 대상의 DC이다.
	// 7번 인자는 출력할 대상의 이미지상에서의 그릴 시작점 X이다.
	// 8번 인자는 출력할 대상의 이미지상에서의 그릴 시작점 Y이다.
	// 마지막 인자는 복사 방법이다.
	// SRCCOPY : 이 DC를 대상 DC에 복사해준다.
	// BLACKNESS : 대상영역을 Black으로 칠한다.
	// WHITENESS : 대상영역을 White으로 칠한다.

	/*
		 이미지의 안 쓰는 색을 뺴내는 방법
		 TransparentBlt : 이미지의 시작지점만 하는 것이 아니라 도착지점까지 지정해주어서 사이즈를 지정할 수 있다.
		 m_iColorKey만 화면에서 색을 빼낼 수 있다. 안쓰는 색들을 적용한 파일들
		 이미지의 시작지점과 도착지점을 정해서 범위가 생성됨 :
		 애니메이션 설정 + 컬러키값을 지정해서 뺄셈도 가능 : 확대 안하기 때문에 같은 크기로 설정
	*/
	if (!m_bColorKey)
		BitBlt(hDC, fDestX, fDestY, fSizeX, fSizeY, m_vecDC[iFrame], fSrcX, fSrcY, SRCCOPY);

	else
		TransparentBlt(hDC, fDestX, fDestY, fSizeX, fSizeY, m_vecDC[iFrame], fSrcX, fSrcY, fSizeX, fSizeY, m_iColorKey);
}

void CTexture::Render(HDC hDC, const Position& tDestPos,
	const Position& tSrcPos, const Size& tSize, int iFrame)
{

	// 컬러 키 사용 여부를 분리해서 BitBlt과 TransparentBlt를 나눈다. 
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

	// 파일 경로를 저장한다.
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
