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

private:// 타일의 x,y갯수
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
							타일을 안쓴다 : 전체 타일 수가 0이 아닐 떄만 true를 리턴한다.
						 */
	{
		return m_iTileCount != 0;
	}


public:
	// 타일을 생성한다. 
	bool CreateTile(int iNumX, int iNumY, const Size& tTileSize,
		const string& strTileTexKey = "",
		const TCHAR* pBaseTile = nullptr,
		const string& strPath = TEXTURE_PATH);

	// 타일을 가져온다. 
	void ChangeTileOption(Position tPos,
		TILE_OPTION eOption);
	class CTile* GetTile(const Position& tPos);

	// ★ 타일의 해당 인덱스를 가져온다.
	int GetTileIndexX(float x);
	int GetTileIndexY(float y);
	int GetTileIndex(Position tPos);

	// ★ 타일이 벽인지 체크한다. 
	bool CheckWallW(float& fResult, RectInfo& tTileRC, CObj* pObj, int iDir);
	bool CheckWallH(float& fResult, RectInfo& tTileRC, CObj* pObj, int iDir);
	/*
	   두개의 좌표를 넣었을 떄 tStart, tEnd
	   두개의 min max값을 넣었을 때 그 범위안에 갈 수 없는 타일이 있는지 없는지를 판단하는 함수
	*/

	/*
	   pObj : 어떤 오브젝트가 들어와 있는가?
	   tOutput : 이미 형성된 사각형 포인트 정보
	   tTileRC : 충돌되는 타일 대상 포인트 정보
	   교집합을 알아본다.
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

