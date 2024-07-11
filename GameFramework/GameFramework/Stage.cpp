#include "Stage.h"
#include "ColliderRect.h"
#include "Animation.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "Tile.h"
#include "Camera.h"
#include "Solid.h"

CStage::CStage()
{
	m_pTileList = nullptr;
	m_iNumX = 0;
	m_iNumY = 0;
	m_iTileCount = 0;
	m_iStartX = 0;
	m_iStartY = 0;
	m_iEndX = 0;
	m_iEndY = 0;
}

CStage::CStage(const CStage& obj) :
	CObj(obj)
{
}

CStage::~CStage()
{
	for (int i = 0; i < m_iTileCount; ++i)
	{
		SAFE_RELEASE(m_pTileList[i]);
	}

	SAFE_DELETE_ARRAY(m_pTileList);
}

bool CStage::CreateTile(int iNumX, int iNumY, const Size& tTileSize,
	const string& strTileTexKey, const TCHAR* pBaseTile,
	const string& strPath)
{
	if (m_pTileList)
	{
		for (int i = 0; i < m_iTileCount; ++i)
		{
			SAFE_RELEASE(m_pTileList[i]);
		}

		SAFE_DELETE_ARRAY(m_pTileList);
	}

	m_iNumX = iNumX;
	m_iNumY = iNumY;
	m_iTileCount = m_iNumX * m_iNumY;
	m_tTileSize = tTileSize;
	SetSize(m_tTileSize.x * m_iNumX, m_tTileSize.y * m_iNumY);

	m_pTileList = new CTile * [m_iTileCount];

	for (int y = 0; y < m_iNumY; ++y)
	{
		for (int x = 0; x < m_iNumX; ++x)
		{
			int	idx = y * m_iNumX + x;

			m_pTileList[idx] = new CTile;

			m_pTileList[idx]->Init();

			m_pTileList[idx]->SetSize(m_tTileSize);

			m_pTileList[idx]->SetPos(x * m_tTileSize.x,
				y * m_tTileSize.y);

			if (pBaseTile)
				m_pTileList[idx]->SetTexture(strTileTexKey, pBaseTile, strPath);
		}
	}

	return true;
}

void CStage::ChangeTileOption(Position tPos,
	TILE_OPTION eOption)
{
	int	idx = GetTileIndex(tPos);

	if (idx == -1)
		return;

	m_pTileList[idx]->SetTileOption(eOption);
}

CTile* CStage::GetTile(const Position& tPos)
{
	int	idx = GetTileIndex(tPos);

	if (idx == -1)
		return nullptr;

	return m_pTileList[idx];
}

int CStage::GetTileIndexX(float x)
{
	x -= m_tPos.x;

	// 위치정보를 타일의 가로, 세로 크기로 나누어서
	// 가로, 세로가 1, 1 크기의 공간으로 변환한다.
	x /= m_tTileSize.x;

	int	iX;
	iX = x;

	if (iX < 0 || iX >= m_iNumX)
		return -1;

	return iX;
}

int CStage::GetTileIndexY(float y)
{
	y -= m_tPos.y;

	// 위치정보를 타일의 가로, 세로 크기로 나누어서
	// 가로, 세로가 1, 1 크기의 공간으로 변환한다.
	y /= m_tTileSize.y;

	int	iY;
	iY = y;

	if (iY < 0 || iY >= m_iNumY)
		return -1;

	return iY;
}

int CStage::GetTileIndex(Position tPos)
{
	tPos -= m_tPos;

	// 위치정보를 타일의 가로, 세로 크기로 나누어서
	// 가로, 세로가 1, 1 크기의 공간으로 변환한다.
	tPos /= m_tTileSize;

	int	iX, iY;
	iX = tPos.x;
	iY = tPos.y;

	if (iX < 0 || iX >= m_iNumX || iY < 0 || iY >= m_iNumY)
		return -1;

	return iY * m_iNumX + iX;
}

bool CStage::CheckWallW(float& fResult, RectInfo& tTileRC,
	CObj* pObj, int iDir)// 좌우 이동
{
	// ★ 0이면 충돌하지 않은 경우 
	if (iDir == 0)
		return false;

	// 오브젝트의 위치, 피봇, 크기를 이용하여 사각형
	// 정보를 만든다.
	Position	tPos = pObj->GetPos();
	Position	tPivot = pObj->GetPivot();
	Size tSize = pObj->GetTileColSize();

	// 두점을 잡는다.
	Position	tStart, tEnd;

	// 왼쪽으로 이동할때
	if (iDir == -1)
	{
		// ★ 충돌체 끝점을 구한다. 
		tStart = tPos - tPivot * tSize;
		tEnd = tPos + (Position(1.f, 1.f) - tPivot) * tSize;
		tEnd.x = tStart.x;
	}

	// 오른쪽으로 이동할때
	else
	{
		tEnd = tPos + (Position(1.f, 1.f) - tPivot) * tSize;
		tStart = tPos - tPivot * tSize;
		tStart.x = tEnd.x;
	}

	// 타일의 인덱스를 구한다. 
	int	iX, iStartY, iEndY;
	/*
		iX : X인덱스
		iStartY : 충돌에 속한 타일의 Y인덱스 (위)
		iEndY : 충돌에 속한 타일의 Y인덱스 (아래)
	*/
	iX = GetTileIndexX(tStart.x);
	iStartY = GetTileIndexY(tStart.y);
	iEndY = GetTileIndexY(tEnd.y);

	// 타일갯수가 0보다 작으면 에러 
	if (iX < 0)
		return false;

	// 타일의 총 갯수를 넘는 넘버는 없다.
	else if (iX >= m_iNumX)
		return false;

	// 충돌에 속한 타일의 iStartY의 값이 0보다 작게 되면 0으로 생각한다. 
	if (iStartY < 0)
		iStartY = 0;

	// 타일의 iStartY는 Y축의 길이를 넘을 수 없다.
	else if (iStartY >= m_iNumY)
		return false;

	// 충돌에 속한 타일의 iEndY이 0일 수 없다.( 그전에  iStartY이 있다)
	if (iEndY < 0)
		return false;

	// 타일의 Y축의 길이보다 iEndY의 값이 조금 넘는다면, -1을 해서 경계를 넘지 않도록 한다. 
	else if (iEndY >= m_iNumY)
		iEndY = m_iNumY - 1;

	// iStartY와 iEndY사이에 있는 모든 y값들을 대입한다.
	for (int y = iStartY; y <= iEndY; ++y)
	{
		// X,Y의 인덱스 값을 이용해서 실제 해당하는 타일의 번호를 구한다. 
		int	idx = y * m_iNumX + iX;

		// 타일의 옵션이 벽이다. 
		if (m_pTileList[idx]->GetTileOption() == TO_WALL)
		{
			Position	tTilePos = m_pTileList[idx]->GetPos();// 타일의 위치좌표를 가져온다. 
			Size tTileSize = m_pTileList[idx]->GetSize();// 해당 타일의 사이즈를 가져온다

			// 타일 사각형의 letf, top, right, bottom을 구한다. 
			tTileRC.l = tTilePos.x;
			tTileRC.t = tTilePos.y;
			tTileRC.r = tTilePos.x + tTileSize.x;
			tTileRC.b = tTilePos.y + tTileSize.y;

			// 교집합을 계산하는 것이 아니라 단순한 좌표값만 계산한다. fResult
			// 이동방향이 1이면, 오른쪽으로 이동할 떄 
			if (iDir == 1)
				fResult = tTileRC.l - tStart.x - 0.1f;
			// 이동방향이 -1이면, 왼쪽으로 이동할 때 
			else
				fResult = tTileRC.r - tStart.x + 0.1f;

			return true;
		}
	}

	return false;
}

bool CStage::CheckWallH(float& fResult, RectInfo& tTileRC,
	CObj* pObj, int iDir)
{
	if (iDir == 0)
		return false;

	// 오브젝트의 위치, 피봇, 크기를 이용하여 사각형
	// 정보를 만든다.
	Position	tPos = pObj->GetPos();
	Position	tPivot = pObj->GetPivot();
	Size tSize = pObj->GetTileColSize();

	Position	tStart, tEnd;

	// 위쪽으로 이동할때
	if (iDir == -1)
	{
		tStart = tPos - tPivot * tSize;
		tEnd = tPos + (Position(1.f, 1.f) - tPivot) * tSize;
		tEnd.y = tStart.y;
	}

	// 아래쪽으로 이동할때
	else
	{
		tEnd = tPos + (Position(1.f, 1.f) - tPivot) * tSize;
		tStart = tPos - tPivot * tSize;
		tStart.y = tEnd.y;
	}

	int	iY, iStartX, iEndX;
	iY = GetTileIndexY(tStart.y);
	iStartX = GetTileIndexX(tStart.x);
	iEndX = GetTileIndexX(tEnd.x);

	if (iY < 0)
		return false;

	else if (iY >= m_iNumY)
		return false;

	if (iStartX < 0)
		iStartX = 0;

	else if (iStartX >= m_iNumX)
		return false;

	if (iEndX < 0)
		return false;

	else if (iEndX >= m_iNumX)
		iEndX = m_iNumX - 1;

	for (int x = iStartX; x <= iEndX; ++x)
	{
		int	idx = iY * m_iNumX + x;

		if (m_pTileList[idx]->GetTileOption() == TO_WALL)
		{
			Position	tTilePos = m_pTileList[idx]->GetPos();
			Size tTileSize = m_pTileList[idx]->GetSize();

			tTileRC.l = tTilePos.x;
			tTileRC.t = tTilePos.y;
			tTileRC.r = tTilePos.x + tTileSize.x;
			tTileRC.b = tTilePos.y + tTileSize.y;

			if (iDir == 1)
				fResult = tTileRC.t - tStart.y - 0.1f;

			else
				fResult = tTileRC.b - tStart.y + 0.1f;

			return true;
		}
	}

	return false;
}

void CStage::Start()
{
	CObj::Start();

	m_pScene->SetStage(this); // ★ 씬정보 에서 스테이지정보를 이 클래스로 지정한다.

	for (int y = 0; y < m_iNumY; ++y)
	{
		for (int x = 0; x < m_iNumX; ++x)
		{
			int	idx = y * m_iNumX + x; // 타일의 인덱스를 구한다. 
			m_pTileList[idx]->Start(); // ★ 이 타일에서 무엇을 할 것인가 ? 
		}
	}
}

bool CStage::Init()
{
	if (!CObj::Init())
		return false;


	return true;
}

void CStage::Update(float fTime)
{
	CObj::Update(fTime);

	// 카메라 위치 
	Position	tCameraPos = GET_SINGLE(CCamera)->GetPos();
	// 카메라의 해상도 
	Size	tResolution = GET_SINGLE(CCamera)->GetResolution();
	// 카메라의 해상도 + 카메라의pos -> RightBottom
	Position	tRB = tCameraPos + tResolution;

	// 타일의 인덱스 구하기 
	m_iStartX = GetTileIndexX(tCameraPos.x);
	m_iStartY = GetTileIndexY(tCameraPos.y);

	// RightBottom을 활용해서 카메라가 플레이어 오브젝트 위치를 찾는다.  
	tRB -= m_tPos;

	// RightBottom 에 타일의 사이즈를 나눈다. 
	tRB /= m_tTileSize;

	// 카메라의 RightBottom을 m_iEndX 와 m_iEndY라고 한다. 
	m_iEndX = tRB.x;
	m_iEndY = tRB.y;

	// 벗어나면 안되기 때문에 -1을 해준다. 
	if (m_iEndX >= m_iNumX)
		m_iEndX = m_iNumX - 1;

	if (m_iEndY >= m_iNumY)
		m_iEndY = m_iNumY - 1;

	// 카메라의 영향을 받는 충돌하는 벽들의 인덱스를 구한다. 
	for (int y = m_iStartY; y <= m_iEndY; ++y)
	{
		for (int x = m_iStartX; x <= m_iEndX; ++x)
		{
			// x축 y축을 전부 돌면서 인덱스를 구한다. 
			int	idx = y * m_iNumX + x;

			// 타일 상태들을 업데이트 한다. 
			m_pTileList[idx]->Update(fTime);
		}
	}
}

void CStage::LateUpdate(float fTime)
{
	CObj::LateUpdate(fTime);

	for (int y = m_iStartY; y <= m_iEndY; ++y)
	{
		for (int x = m_iStartX; x <= m_iEndX; ++x)
		{
			int	idx = y * m_iNumX + x;

			m_pTileList[idx]->LateUpdate(fTime);
		}
	}
}

void CStage::Collision(float fTime)
{
	CObj::Collision(fTime);
}

void CStage::PrevRender(HDC hDC, float fTime)
{
	CObj::PrevRender(hDC, fTime);

	for (int y = m_iStartY; y <= m_iEndY; ++y)
	{
		for (int x = m_iStartX; x <= m_iEndX; ++x)
		{
			int	idx = y * m_iNumX + x;

			m_pTileList[idx]->PrevRender(hDC, fTime);
		}
	}
}

void CStage::Render(HDC hDC, float fTime)
{
	CObj::Render(hDC, fTime);

	for (int y = m_iStartY; y <= m_iEndY; ++y)
	{
		for (int x = m_iStartX; x <= m_iEndX; ++x)
		{
			int	idx = y * m_iNumX + x;

			m_pTileList[idx]->SetPos(m_tPos.x + x * m_tTileSize.x,
				m_tPos.y + y * m_tTileSize.y);

			m_pTileList[idx]->Render(hDC, fTime);
		}
	}
}

void CStage::PostRender(HDC hDC, float fTime)
{
	CObj::PostRender(hDC, fTime);

	for (int y = m_iStartY; y <= m_iEndY; ++y)
	{
		for (int x = m_iStartX; x <= m_iEndX; ++x)
		{
			int	idx = y * m_iNumX + x;

			m_pTileList[idx]->PostRender(hDC, fTime);
		}
	}
}

CStage* CStage::Clone() const
{
	return new CStage(*this);
}

void CStage::Save(FILE* pFile)
{
	CObj::Save(pFile);

	fwrite(&m_iNumX, sizeof(int), 1, pFile);
	fwrite(&m_iNumY, sizeof(int), 1, pFile);
	fwrite(&m_iTileCount, sizeof(int), 1, pFile);
	fwrite(&m_tTileSize, sizeof(Size), 1, pFile);

	for (int i = 0; i < m_iTileCount; ++i)
	{
		m_pTileList[i]->Save(pFile);
	}
}

void CStage::Load(FILE* pFile)
{
	for (int i = 0; i < m_iTileCount; ++i)
	{
		SAFE_RELEASE(m_pTileList[i]);
	}

	SAFE_DELETE_ARRAY(m_pTileList);

	CObj::Load(pFile);

	fread(&m_iNumX, sizeof(int), 1, pFile);
	fread(&m_iNumY, sizeof(int), 1, pFile);
	fread(&m_iTileCount, sizeof(int), 1, pFile);
	fread(&m_tTileSize, sizeof(Size), 1, pFile);

	m_pTileList = new CTile * [m_iTileCount];

	for (int i = 0; i < m_iTileCount; ++i)
	{
		m_pTileList[i] = new CTile;
		m_pTileList[i]->Load(pFile);
	}
}
