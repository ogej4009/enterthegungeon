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

	// ��ġ������ Ÿ���� ����, ���� ũ��� �����
	// ����, ���ΰ� 1, 1 ũ���� �������� ��ȯ�Ѵ�.
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

	// ��ġ������ Ÿ���� ����, ���� ũ��� �����
	// ����, ���ΰ� 1, 1 ũ���� �������� ��ȯ�Ѵ�.
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

	// ��ġ������ Ÿ���� ����, ���� ũ��� �����
	// ����, ���ΰ� 1, 1 ũ���� �������� ��ȯ�Ѵ�.
	tPos /= m_tTileSize;

	int	iX, iY;
	iX = tPos.x;
	iY = tPos.y;

	if (iX < 0 || iX >= m_iNumX || iY < 0 || iY >= m_iNumY)
		return -1;

	return iY * m_iNumX + iX;
}

bool CStage::CheckWallW(float& fResult, RectInfo& tTileRC,
	CObj* pObj, int iDir)// �¿� �̵�
{
	// �� 0�̸� �浹���� ���� ��� 
	if (iDir == 0)
		return false;

	// ������Ʈ�� ��ġ, �Ǻ�, ũ�⸦ �̿��Ͽ� �簢��
	// ������ �����.
	Position	tPos = pObj->GetPos();
	Position	tPivot = pObj->GetPivot();
	Size tSize = pObj->GetTileColSize();

	// ������ ��´�.
	Position	tStart, tEnd;

	// �������� �̵��Ҷ�
	if (iDir == -1)
	{
		// �� �浹ü ������ ���Ѵ�. 
		tStart = tPos - tPivot * tSize;
		tEnd = tPos + (Position(1.f, 1.f) - tPivot) * tSize;
		tEnd.x = tStart.x;
	}

	// ���������� �̵��Ҷ�
	else
	{
		tEnd = tPos + (Position(1.f, 1.f) - tPivot) * tSize;
		tStart = tPos - tPivot * tSize;
		tStart.x = tEnd.x;
	}

	// Ÿ���� �ε����� ���Ѵ�. 
	int	iX, iStartY, iEndY;
	/*
		iX : X�ε���
		iStartY : �浹�� ���� Ÿ���� Y�ε��� (��)
		iEndY : �浹�� ���� Ÿ���� Y�ε��� (�Ʒ�)
	*/
	iX = GetTileIndexX(tStart.x);
	iStartY = GetTileIndexY(tStart.y);
	iEndY = GetTileIndexY(tEnd.y);

	// Ÿ�ϰ����� 0���� ������ ���� 
	if (iX < 0)
		return false;

	// Ÿ���� �� ������ �Ѵ� �ѹ��� ����.
	else if (iX >= m_iNumX)
		return false;

	// �浹�� ���� Ÿ���� iStartY�� ���� 0���� �۰� �Ǹ� 0���� �����Ѵ�. 
	if (iStartY < 0)
		iStartY = 0;

	// Ÿ���� iStartY�� Y���� ���̸� ���� �� ����.
	else if (iStartY >= m_iNumY)
		return false;

	// �浹�� ���� Ÿ���� iEndY�� 0�� �� ����.( ������  iStartY�� �ִ�)
	if (iEndY < 0)
		return false;

	// Ÿ���� Y���� ���̺��� iEndY�� ���� ���� �Ѵ´ٸ�, -1�� �ؼ� ��踦 ���� �ʵ��� �Ѵ�. 
	else if (iEndY >= m_iNumY)
		iEndY = m_iNumY - 1;

	// iStartY�� iEndY���̿� �ִ� ��� y������ �����Ѵ�.
	for (int y = iStartY; y <= iEndY; ++y)
	{
		// X,Y�� �ε��� ���� �̿��ؼ� ���� �ش��ϴ� Ÿ���� ��ȣ�� ���Ѵ�. 
		int	idx = y * m_iNumX + iX;

		// Ÿ���� �ɼ��� ���̴�. 
		if (m_pTileList[idx]->GetTileOption() == TO_WALL)
		{
			Position	tTilePos = m_pTileList[idx]->GetPos();// Ÿ���� ��ġ��ǥ�� �����´�. 
			Size tTileSize = m_pTileList[idx]->GetSize();// �ش� Ÿ���� ����� �����´�

			// Ÿ�� �簢���� letf, top, right, bottom�� ���Ѵ�. 
			tTileRC.l = tTilePos.x;
			tTileRC.t = tTilePos.y;
			tTileRC.r = tTilePos.x + tTileSize.x;
			tTileRC.b = tTilePos.y + tTileSize.y;

			// �������� ����ϴ� ���� �ƴ϶� �ܼ��� ��ǥ���� ����Ѵ�. fResult
			// �̵������� 1�̸�, ���������� �̵��� �� 
			if (iDir == 1)
				fResult = tTileRC.l - tStart.x - 0.1f;
			// �̵������� -1�̸�, �������� �̵��� �� 
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

	// ������Ʈ�� ��ġ, �Ǻ�, ũ�⸦ �̿��Ͽ� �簢��
	// ������ �����.
	Position	tPos = pObj->GetPos();
	Position	tPivot = pObj->GetPivot();
	Size tSize = pObj->GetTileColSize();

	Position	tStart, tEnd;

	// �������� �̵��Ҷ�
	if (iDir == -1)
	{
		tStart = tPos - tPivot * tSize;
		tEnd = tPos + (Position(1.f, 1.f) - tPivot) * tSize;
		tEnd.y = tStart.y;
	}

	// �Ʒ������� �̵��Ҷ�
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

	m_pScene->SetStage(this); // �� ������ ���� �������������� �� Ŭ������ �����Ѵ�.

	for (int y = 0; y < m_iNumY; ++y)
	{
		for (int x = 0; x < m_iNumX; ++x)
		{
			int	idx = y * m_iNumX + x; // Ÿ���� �ε����� ���Ѵ�. 
			m_pTileList[idx]->Start(); // �� �� Ÿ�Ͽ��� ������ �� ���ΰ� ? 
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

	// ī�޶� ��ġ 
	Position	tCameraPos = GET_SINGLE(CCamera)->GetPos();
	// ī�޶��� �ػ� 
	Size	tResolution = GET_SINGLE(CCamera)->GetResolution();
	// ī�޶��� �ػ� + ī�޶���pos -> RightBottom
	Position	tRB = tCameraPos + tResolution;

	// Ÿ���� �ε��� ���ϱ� 
	m_iStartX = GetTileIndexX(tCameraPos.x);
	m_iStartY = GetTileIndexY(tCameraPos.y);

	// RightBottom�� Ȱ���ؼ� ī�޶� �÷��̾� ������Ʈ ��ġ�� ã�´�.  
	tRB -= m_tPos;

	// RightBottom �� Ÿ���� ����� ������. 
	tRB /= m_tTileSize;

	// ī�޶��� RightBottom�� m_iEndX �� m_iEndY��� �Ѵ�. 
	m_iEndX = tRB.x;
	m_iEndY = tRB.y;

	// ����� �ȵǱ� ������ -1�� ���ش�. 
	if (m_iEndX >= m_iNumX)
		m_iEndX = m_iNumX - 1;

	if (m_iEndY >= m_iNumY)
		m_iEndY = m_iNumY - 1;

	// ī�޶��� ������ �޴� �浹�ϴ� ������ �ε����� ���Ѵ�. 
	for (int y = m_iStartY; y <= m_iEndY; ++y)
	{
		for (int x = m_iStartX; x <= m_iEndX; ++x)
		{
			// x�� y���� ���� ���鼭 �ε����� ���Ѵ�. 
			int	idx = y * m_iNumX + x;

			// Ÿ�� ���µ��� ������Ʈ �Ѵ�. 
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
