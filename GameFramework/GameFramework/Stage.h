#pragma once
#include "Obj.h"


class CStage :
	public CObj
{
	DECLARE_OBJECT

protected:
	CStage();
	CStage(const CStage& obj);
	~CStage();

private:// Ÿ���� x,y����
	int		m_iNumX;
	int		m_iNumY;
	int		m_iTileCount;
	Size	m_tTileSize;
	class CTile** m_pTileList;
	int		m_iStartX;
	int		m_iStartY;
	int		m_iEndX;
	int		m_iEndY;

public:
	bool IsTile()	const/*
							Ÿ���� �Ⱦ��� : ��ü Ÿ�� ���� 0�� �ƴ� ���� true�� �����Ѵ�.
						 */
	{
		return m_iTileCount != 0;
	}


public:
	// Ÿ���� �����Ѵ�. 
	bool CreateTile(int iNumX, int iNumY, const Size& tTileSize,
		const string& strTileTexKey = "",
		const TCHAR* pBaseTile = nullptr,
		const string& strPath = TEXTURE_PATH);

	// Ÿ���� �����´�. 
	void ChangeTileOption(Position tPos,
		TILE_OPTION eOption);
	class CTile* GetTile(const Position& tPos);

	// �� Ÿ���� �ش� �ε����� �����´�.
	int GetTileIndexX(float x);
	int GetTileIndexY(float y);
	int GetTileIndex(Position tPos);

	// �� Ÿ���� ������ üũ�Ѵ�. 
	bool CheckWallW(float& fResult, RectInfo& tTileRC, CObj* pObj, int iDir);
	bool CheckWallH(float& fResult, RectInfo& tTileRC, CObj* pObj, int iDir);
	/*
	   �ΰ��� ��ǥ�� �־��� �� tStart, tEnd
	   �ΰ��� min max���� �־��� �� �� �����ȿ� �� �� ���� Ÿ���� �ִ��� �������� �Ǵ��ϴ� �Լ�
	*/

	/*
	   pObj : � ������Ʈ�� ���� �ִ°�?
	   tOutput : �̹� ������ �簢�� ����Ʈ ����
	   tTileRC : �浹�Ǵ� Ÿ�� ��� ����Ʈ ����
	   �������� �˾ƺ���.
	*/

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float fTime);
	virtual void LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PrevRender(HDC hDC, float fTime);
	virtual void Render(HDC hDC, float fTime);
	virtual void PostRender(HDC hDC, float fTime);
	virtual CStage* Clone()	const;
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

